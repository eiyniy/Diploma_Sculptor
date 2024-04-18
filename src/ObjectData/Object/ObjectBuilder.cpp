#include <ObjectBuilder.hpp>

#include <IBuilder.hpp>
#include <Object.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

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
    const auto vectorSize = vertices.size();

    instance->verticesUnionSize = vectorSize * 3;
    instance->verticesUnionStep = 3;

    if (!colors.empty()) {
        instance->verticesUnionSize += vectorSize * 3;
        instance->verticesUnionStep += 3;
    }
    if (!textureCoords.empty()) {
        instance->verticesUnionSize += vectorSize * 2;
        instance->verticesUnionStep += 2;
    }
}

void ObjectBuilder::mergeVertices()
{
    const auto vectorSize = vertices.size();

    instance->verticesUnion.resize(instance->verticesUnionSize);

    for (std::size_t i = 0; i < vectorSize; ++i) {
        const auto j = i * instance->verticesUnionStep;
        const auto& vertex = vertices[i];

        instance->verticesUnion[j] = vertex.x;
        instance->verticesUnion[j + 1] = vertex.y;
        instance->verticesUnion[j + 2] = vertex.z;

        auto k = 3;

        if (!colors.empty()) {
            const auto& colorVertex = colors.at(i);

            instance->verticesUnion[j + k] = colorVertex.r;
            instance->verticesUnion[j + k + 1] = colorVertex.g;
            instance->verticesUnion[j + k + 2] = colorVertex.b;

            k += 3;
        }

        if (!textureCoords.empty()) {
            const auto& textureVertex = textureCoords.at(i);

            instance->verticesUnion[j + k] = textureVertex.x;
            instance->verticesUnion[j + k + 1] = textureVertex.y;

            k += 2;
        }
    }
}

void ObjectBuilder::mergeIndices()
{
    if (!indices.empty()) {
        instance->indicesUnionSize = indices.size() * 3;
        instance->indicesUnion.resize(instance->indicesUnionSize);

        for (std::size_t i = 0; i < indices.size(); ++i) {
            const auto j = i * 3;
            const auto& index = indices.at(i);

            instance->indicesUnion[j] = index.x;
            instance->indicesUnion[j + 1] = index.y;
            instance->indicesUnion[j + 2] = index.z;
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
        && isShaderProgramFinished() && isMerged && isVAOSetup;
}

void ObjectBuilder::init(const std::vector<glm::vec3>& _vertices)
{
    vertices = _vertices;
    instance->verticesSize = _vertices.size();

    isInited = true;
}

// TODO: Add check to already existing of colors
void ObjectBuilder::addColors(const std::vector<glm::vec3>& _colors)
{
    if (_colors.size() != vertices.size()) {
        throw std::logic_error(
            "Can't add colors to ObjectBuilder. They have incorrect size.");
    }

    colors = _colors;
}

void ObjectBuilder::addTextureCoords(
    const std::vector<glm::vec2>& _textureCoords)
{
    if (_textureCoords.size() != vertices.size()) {
        throw std::logic_error(
            "Can't add texture coords to ObjectBuilder. They "
            "have incorrect size.");
    }

    textureCoords = _textureCoords;
}

void ObjectBuilder::addIndices(const std::vector<glm::vec<3, GLuint>>& _indices)
{
    indices = _indices;
    instance->_hasIndices = true;
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

    mergeVertices();

    mergeIndices();

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
