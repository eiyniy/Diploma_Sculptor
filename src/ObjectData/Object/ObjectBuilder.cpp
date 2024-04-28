#include <ObjectBuilder.hpp>

#include <IBuilder.hpp>
#include <Object.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

Entry::Entry()
    : vertex() {};

Entry::Entry(
    const glm::vec4& _vertex,
    const std::optional<glm::vec3>& _nVertex,
    const std::optional<glm::vec2>& _tVertex)
    : vertex(_vertex)
    , nVertex(_nVertex)
    , tVertex(_tVertex)
{
}

bool Entry::operator==(const Entry& entry) const
{
    return vertex == entry.vertex && tVertex == entry.tVertex
        && nVertex == entry.nVertex;
}

[[nodiscard]] size_t Entry::hash() const
{
    size_t hash {};
    auto shift = 0;

    hash ^= std::hash<float>()(vertex.x) << shift++;
    hash ^= std::hash<float>()(vertex.y) << shift++;
    hash ^= std::hash<float>()(vertex.z) << shift++;
    hash ^= std::hash<float>()(vertex.w) << shift++;

    if (nVertex.has_value()) {
        hash ^= std::hash<float>()(nVertex->x) << shift++;
        hash ^= std::hash<float>()(nVertex->y) << shift++;
        hash ^= std::hash<float>()(nVertex->z) << shift++;
    }

    if (tVertex.has_value()) {
        hash ^= std::hash<float>()(tVertex->x) << shift++;
        hash ^= std::hash<float>()(tVertex->y) << shift++;
    }

    return hash;
}

size_t Entry::HashFunction::operator()(const Entry& entry) const
{
    return entry.hash();
}

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
    glGenBuffers(1, &instance->nVerticesVBO);
    glGenBuffers(1, &instance->EBO);
}

void ObjectBuilder::transform()
{
    throwIfNotInited("Can't merge vertices. ");

    std::map<size_t, size_t> entriesHashId;
    std::vector<Entry> entries;

    instance->indicesSize = triangles->size() * 3;
    instance->indices.resize(instance->indicesSize);

    size_t indicesPos = 0;
    size_t entriesPos = 0;

    for (const auto& triangle : *triangles) {
        const auto vertexIdsCount = triangle.cGetVertexIdsCount();

        for (auto i = 0; i < vertexIdsCount; ++i) {
            const auto vertexIds = triangle.cGetVertexIds(i);

            const auto vertexId = vertexIds.cGetVertexId() - 1;
            const auto vertex = vertices->at(vertexId);

            auto nVertexId = vertexIds.cGetNormalVertexId();
            std::optional<glm::vec3> nVertex {};
            if (nVertexId.has_value() && nVertices != nullptr) {
                nVertex = nVertices->at(--*nVertexId);
            }

            auto tVertexId = vertexIds.cGetTextureVertexId();
            std::optional<glm::vec2> tVertex {};
            if (tVertexId.has_value() && tVertices != nullptr) {
                tVertex = tVertices->at(--*tVertexId);
            }

            const Entry entry { vertex, nVertex, tVertex };

            const auto hash = entry.hash();
            if (!entriesHashId.contains(hash)) {
                entriesHashId[hash] = entriesPos;
                entries.push_back(entry);

                instance->indices[indicesPos++] = entriesPos++;
            } else {
                instance->indices[indicesPos++] = entriesHashId.at(hash);
            }
        }
    }

    instance->trVerticesSize = entries.size() * 4;
    instance->trVertices.resize(instance->trVerticesSize);

    instance->trTVerticesSize = entries.size() * 2;
    instance->trTVertices.resize(instance->trTVerticesSize);

    instance->trNVerticesSize = entries.size() * 3;
    instance->trNVertices.resize(instance->trNVerticesSize);

    size_t verticesPos = 0;
    size_t tVerticesPos = 0;
    size_t nVerticesPos = 0;

    for (auto&& entry : entries) {
        instance->trVertices[verticesPos++] = entry.vertex.x;
        instance->trVertices[verticesPos++] = entry.vertex.y;
        instance->trVertices[verticesPos++] = entry.vertex.z;
        instance->trVertices[verticesPos++] = entry.vertex.w;

        if (entry.tVertex.has_value()) {
            instance->trTVertices[tVerticesPos++] = entry.tVertex->x;
            instance->trTVertices[tVerticesPos++] = entry.tVertex->y;
        }

        if (entry.nVertex.has_value()) {
            instance->trNVertices[nVerticesPos++] = entry.nVertex->x;
            instance->trNVertices[nVerticesPos++] = entry.nVertex->y;
            instance->trNVertices[nVerticesPos++] = entry.nVertex->z;
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

void ObjectBuilder::addNVertices(
    std::unique_ptr<std::vector<glm::vec3>> _nVertices)
{
    const std::string error = "Can't add normals to ObjectBuilder. ";

    throwIfNotInited(error);

    if (nVertices != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    nVertices = std::move(_nVertices);
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

    glBindBuffer(GL_ARRAY_BUFFER, instance->verticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->trVerticesSize * sizeof(GLfloat)),
        instance->trVertices.data(),
        GL_STATIC_DRAW);
    instance->shaderPrograms.at(instance->currentShaderProgramName)
        ->enableAttribute(ShaderProgram::positionAName);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (tVertices != nullptr) {
        glBindBuffer(GL_ARRAY_BUFFER, instance->tVerticesVBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                instance->trTVerticesSize * sizeof(GLfloat)),
            instance->trTVertices.data(),
            GL_STATIC_DRAW);
        instance->shaderPrograms.at(instance->currentShaderProgramName)
            ->enableAttribute(ShaderProgram::texCoordAName);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (nVertices != nullptr) {
        glBindBuffer(GL_ARRAY_BUFFER, instance->nVerticesVBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                instance->trNVerticesSize * sizeof(GLfloat)),
            instance->trNVertices.data(),
            GL_STATIC_DRAW);
        instance->shaderPrograms.at(instance->currentShaderProgramName)
            ->enableAttribute(ShaderProgram::normalAName);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instance->indicesSize * sizeof(GLuint)),
        instance->indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

    isVAOSetup = true;
}
