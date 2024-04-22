#include <ObjectBuilder.hpp>

#include <IBuilder.hpp>
#include <Object.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <memory>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

ObjectBuilder::ObjectBuilder()
    : isInited(false)
    , isMerged(false)
    , isVAOSetup(false)
{
}

void ObjectBuilder::throwIfNotInited(const std::string& message) const
{
    if (!isInited) {
        throw std::logic_error(
            message + "ObjectBuilder hasn't been initialised");
    }
}

void ObjectBuilder::reset()
{
    IBuilder<Object>::reset();

    isInited = false;
    isMerged = false;
    isVAOSetup = false;
}

void ObjectBuilder::generateBuffers()
{
    glGenVertexArrays(1, &instance->VAO);
    glGenBuffers(1, &instance->VBO);
    if (instance->hasIndices()) {
        glGenBuffers(1, &instance->EBO);
    }
}

void ObjectBuilder::calcVerticesUnionParams()
{
    throwIfNotInited("Can't calculate vertices union params. ");

    const auto trianglesCount = triangles->size();
    const auto verticesCount = vertices->size();

    // TODO: Get rid of magic numbers
    instance->verticesUnionSize = verticesCount * 4;
    instance->verticesUnionStep = 4;

    if (colors != nullptr) {
        instance->verticesUnionSize += verticesCount * 3;
        instance->verticesUnionStep += 3;
    }
    if (textureCoords != nullptr) {
        instance->verticesUnionSize += verticesCount * 2;
        instance->verticesUnionStep += 2;
    }
}

void ObjectBuilder::mergeTriangles()
{
    throwIfNotInited("Can't merge vertices. ");

    instance->verticesUnion.resize(instance->verticesUnionSize);

    instance->indicesUnionSize = triangles->size() * 3;
    instance->indicesUnion.resize(instance->indicesUnionSize);

    size_t indicesUnionPos = 0;
    size_t verticesUnionPos = 0;

    for (auto&& vertex : *vertices) {
        instance->verticesUnion[verticesUnionPos++] = vertex.x;
        instance->verticesUnion[verticesUnionPos++] = vertex.y;
        instance->verticesUnion[verticesUnionPos++] = vertex.z;
        instance->verticesUnion[verticesUnionPos++] = vertex.w;

        verticesUnionPos += instance->verticesUnionStep - 4;
    }

    for (const auto& triangle : *triangles) {
        const auto vertexIdsCount = triangle.cGetVertexIdsCount();

        for (auto i = 0; i < vertexIdsCount; ++i) {
            const auto vertexIds = triangle.cGetVertexIds(i);

            const auto vertexId = vertexIds.cGetVertexId() - 1;

            instance->indicesUnion[indicesUnionPos++] = vertexId;

            /*

            // HACK: Color id must be the as vertx id.
            // Their vectors must have the same size.
            // TODO: Remove or remake color support
            if (colors != nullptr) {
                const auto colorId = vertexIds.cGetVertexId() - 1;
                const auto& color = colors->at(colorId);

                instance->verticesUnion[verticesUnionPos++] = color.r;
                instance->verticesUnion[verticesUnionPos++] = color.g;
                instance->verticesUnion[verticesUnionPos++] = color.b;
            }

            auto tVertexId = vertexIds.cGetTextureVertexId();
            if (textureCoords != nullptr && tVertexId.has_value()) {
                --*tVertexId;

                const auto& tVertex = textureCoords->at(*tVertexId);

                instance->verticesUnion[verticesUnionPos++] = tVertex.x;
                instance->verticesUnion[verticesUnionPos++] = tVertex.y;
            }
            */
        }
    }

    int a = 0;
}

bool ObjectBuilder::isShaderProgramFinished() const
{
    return !instance->shaderPrograms.empty();
}

bool ObjectBuilder::isFinished() const
{
    return IBuilder<Object>::isFinished() && isInited
        && isShaderProgramFinished() && isMerged && isVAOSetup;
}

void ObjectBuilder::init(std::unique_ptr<std::vector<glm::vec4>> _vertices)
{
    if (isInited) {
        throw std::logic_error(
            "Can't init ObjectBuilder. It has already been initialised");
    }

    vertices = std::move(_vertices);

    instance->verticesSize = vertices->size();

    isInited = true;
}

void ObjectBuilder::addTriangles(
    std::unique_ptr<std::vector<Triangle>> _triangles)
{
    // TODO: Move next 5 lines to IBuilder::addBase(std::string error)
    const std::string error = "Can't add triangles to ObjectBuilder. ";

    throwIfNotInited(error);

    if (triangles != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    triangles = std::move(_triangles);
    instance->_hasIndices = true;
}

void ObjectBuilder::addColors(std::unique_ptr<std::vector<glm::vec3>> _colors)
{
    const std::string error = "Can't add colors to ObjectBuilder. ";

    throwIfNotInited(error);

    if (colors != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    if (_colors->size() != vertices->size()) {
        throw std::logic_error(error + "They have incorrect size.");
    }

    colors = std::move(_colors);
}

void ObjectBuilder::addTextureCoords(
    std::unique_ptr<std::vector<glm::vec2>> _textureCoords)
{
    const std::string error = "Can't add texture coords to ObjectBuilder. ";

    throwIfNotInited(error);

    if (textureCoords != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    textureCoords = std::move(_textureCoords);
}

void ObjectBuilder::addShaderProgram(
    std::unique_ptr<ShaderProgram> shaderProgram)
{
    instance->shaderPrograms.insert(
        std::make_pair(shaderProgram->getName(), std::move(shaderProgram)));
}

void ObjectBuilder::addTexture(std::unique_ptr<Texture> texture)
{
    instance->textures.insert(
        std::make_pair(texture->getName(), std::move(texture)));
}

void ObjectBuilder::selectShaderProgram(std::string_view name)
{
    instance->selectShaderProgram(name);
}

void ObjectBuilder::merge()
{
    generateBuffers();

    calcVerticesUnionParams();

    mergeTriangles();

    isMerged = true;
}

void ObjectBuilder::setupVAO()
{
    if (!isMerged) {
        throw std::logic_error(
            "Can't setup VAO. ObjectBuilder haven't been merged.");
    }

    instance->throwIfShaderNotSelected("Can't setup VAO.");

    glBindVertexArray(instance->VAO);

    // TODO: Add separate buffers for vertices, colors, tVertices
    // DO NOT use union
    glBindBuffer(GL_ARRAY_BUFFER, instance->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->verticesUnionSize * sizeof(GLfloat)),
        instance->verticesUnion.data(),
        GL_STATIC_DRAW);

    if (instance->hasIndices()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                instance->indicesUnionSize * sizeof(GLuint)),
            instance->indicesUnion.data(),
            GL_STATIC_DRAW);
    }

    instance->shaderPrograms.at(instance->currentShaderProgramName)
        ->enableAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isVAOSetup = true;
}
