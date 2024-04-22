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
    , isTransformed(false)
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
    isTransformed = false;
    isVAOSetup = false;
}

void ObjectBuilder::generateBuffers()
{
    glGenVertexArrays(1, &instance->VAO);
    glGenBuffers(1, &instance->verticesVBO);
    glGenBuffers(1, &instance->tVerticesVBO);
    glGenBuffers(1, &instance->EBO);
}

void ObjectBuilder::calcVerticesUnionParams()
{
    throwIfNotInited("Can't calculate vertices union params. ");

    // TODO: Get rid of magic numbers
    instance->trVerticesSize = vertices->size() * 4;
    instance->trVerticesStep = 4;

    instance->indicesSize = triangles->size() * 3;

    if (tVertices != nullptr) {
        instance->trTVerticesSize = tVertices->size() * 2;
        instance->trTVerticesStep = 2;
    }
}

void ObjectBuilder::transform()
{
    throwIfNotInited("Can't merge vertices. ");

    instance->trVertices.resize(instance->trVerticesSize);
    instance->trTVertices.resize(instance->trTVerticesSize);
    instance->indices.resize(instance->indicesSize);

    size_t indicesPos = 0;
    size_t verticesPos = 0;
    size_t tVerticesPos = 0;

    for (auto&& vertex : *vertices) {
        instance->trVertices[verticesPos++] = vertex.x;
        instance->trVertices[verticesPos++] = vertex.y;
        instance->trVertices[verticesPos++] = vertex.z;
        instance->trVertices[verticesPos++] = vertex.w;
    }

    for (auto&& tVertex : *tVertices) {
        instance->trTVertices[tVerticesPos++] = tVertex.x;
        instance->trTVertices[tVerticesPos++] = tVertex.y;
    }

    for (const auto& triangle : *triangles) {
        const auto vertexIdsCount = triangle.cGetVertexIdsCount();

        for (auto i = 0; i < vertexIdsCount; ++i) {
            const auto vertexIds = triangle.cGetVertexIds(i);

            const auto vertexId = vertexIds.cGetVertexId() - 1;

            instance->indices[indicesPos++] = vertexId;

            // FIXME: Fix tVertices order
            // https://gamedev.stackexchange.com/questions/102389/how-to-implement-index-buffer-object-ibos-with-texture-coordinates-in-opengl
            /*
            auto tVertexId = vertexIds.cGetTextureVertexId();
            if (tVertices != nullptr && tVertexId.has_value()) {
                --*tVertexId;

                const auto& tVertex = tVertices->at(*tVertexId);

                instance->trTVertices[tVerticesPos++] = tVertex.x;
                instance->trTVertices[tVerticesPos++] = tVertex.y;
            }
            // */
        }
    }
}

bool ObjectBuilder::isShaderProgramFinished() const
{
    return !instance->shaderPrograms.empty();
}

bool ObjectBuilder::isFinished() const
{
    return IBuilder<Object>::isFinished() && isInited
        && isShaderProgramFinished() && isTransformed && isVAOSetup;
}

void ObjectBuilder::init(
    std::unique_ptr<std::vector<glm::vec4>> _vertices,
    std::unique_ptr<std::vector<Triangle>> _triangles)
{
    if (isInited) {
        throw std::logic_error(
            "Can't init ObjectBuilder. It has already been initialised");
    }

    vertices = std::move(_vertices);
    triangles = std::move(_triangles);

    isInited = true;
}

void ObjectBuilder::addTVertices(
    std::unique_ptr<std::vector<glm::vec2>> _tVertices)
{
    const std::string error = "Can't add texture coords to ObjectBuilder. ";

    throwIfNotInited(error);

    if (tVertices != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    tVertices = std::move(_tVertices);
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

    transform();

    isTransformed = true;
}

void ObjectBuilder::setupVAO()
{
    if (!isTransformed) {
        throw std::logic_error(
            "Can't setup VAO. ObjectBuilder haven't been merged.");
    }

    instance->throwIfShaderNotSelected("Can't setup VAO.");

    glBindVertexArray(instance->VAO);

    // TODO: Add separate buffers for vertices, colors, tVertices
    // DO NOT use union
    glBindBuffer(GL_ARRAY_BUFFER, instance->verticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->trVerticesSize * sizeof(GLfloat)),
        instance->trVertices.data(),
        GL_STATIC_DRAW);
    instance->shaderPrograms.at(instance->currentShaderProgramName)
        ->enableAttribute(ShaderProgram::defaultPositionAttributeName);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO: tVerticesVBO
    glBindBuffer(GL_ARRAY_BUFFER, instance->tVerticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->trTVerticesSize * sizeof(GLfloat)),
        instance->trTVertices.data(),
        GL_STATIC_DRAW);
    instance->shaderPrograms.at(instance->currentShaderProgramName)
        ->enableAttribute(ShaderProgram::defaultTexCoordAttributeName);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->indicesSize * sizeof(GLuint)),
        instance->indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

    isVAOSetup = true;
}
