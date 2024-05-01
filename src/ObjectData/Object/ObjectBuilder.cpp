#include <ObjectBuilder.hpp>

#include <Camera.hpp>
#include <IBuilder.hpp>
#include <Object.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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

[[nodiscard]] std::size_t Entry::hash() const
{
    std::size_t hash {};
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

std::size_t Entry::HashFunction::operator()(const Entry& entry) const
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

void ObjectBuilder::transform()
{
    throwIfNotInited("Can't merge vertices. ");

    const auto transformEntriesStart
        = std::chrono::high_resolution_clock::now();

    std::map<std::size_t, std::size_t> entriesHashId;
    std::vector<Entry> entries;

    instance->indices.resize(triangles->size() * 3);

    std::size_t indicesPos = 0;
    std::size_t entriesPos = 0;

    std::cout << "Triangles count x3: " << triangles->size() * 3 << std::endl;

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

    std::cout << "Entries count: " << entries.size() << std::endl;

    const auto transformSeparationStart
        = std::chrono::high_resolution_clock::now();

    instance->trVertices.resize(entries.size() * glm::vec4::length());
    instance->trTVertices.resize(entries.size() * glm::vec2::length());
    instance->trNVertices.resize(entries.size() * glm::vec3::length());

    for (std::size_t entryid = 0,
                     verticesPos = 0,
                     tVerticesPos = 0,
                     nVerticesPos = 0;
         entryid < entries.size();
         ++entryid, verticesPos += 4, tVerticesPos += 2, nVerticesPos += 3) {
        auto&& entry = entries.at(entryid);

        instance->trVertices[verticesPos] = entry.vertex.x;
        instance->trVertices[verticesPos + 1] = entry.vertex.y;
        instance->trVertices[verticesPos + 2] = entry.vertex.z;
        instance->trVertices[verticesPos + 3] = entry.vertex.w;

        if (entry.tVertex.has_value()) {
            instance->trTVertices[tVerticesPos] = entry.tVertex->x;
            instance->trTVertices[tVerticesPos + 1] = entry.tVertex->y;
        }

        if (entry.nVertex.has_value()) {
            instance->trNVertices[nVerticesPos] = entry.nVertex->x;
            instance->trNVertices[nVerticesPos + 1] = entry.nVertex->y;
            instance->trNVertices[nVerticesPos + 2] = entry.nVertex->z;
        }
    }

    std::cout << "Transform entries time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     transformSeparationStart - transformEntriesStart)
              << std::endl;
    std::cout << "Transform separation time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now()
                     - transformSeparationStart)
              << std::endl;

    isTransformed = true;
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

    glGenVertexArrays(1, &instance->VAO);
    glGenBuffers(1, &instance->verticesVBO);
    glGenBuffers(1, &instance->tVerticesVBO);
    glGenBuffers(1, &instance->nVerticesVBO);
    glGenBuffers(1, &instance->EBO);

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

void ObjectBuilder::setupVAO()
{
    if (!isTransformed) {
        throw std::logic_error(
            "Can't setup VAO. ObjectBuilder haven't been merged.");
    }

    instance->throwIfShaderNotSelected("Can't setup VAO.");

    glBindVertexArray(instance->VAO);

    instance->bindVerticesVBO();
    instance->shaderPrograms.at(instance->currentShaderProgramName)
        ->enableAttribute(ShaderProgram::positionAName);
    instance->unbindVBO();

    if (tVertices != nullptr) {
        instance->bindTVerticesVBO();
        instance->shaderPrograms.at(instance->currentShaderProgramName)
            ->enableAttribute(ShaderProgram::texCoordAName);
        instance->unbindVBO();
    }

    if (nVertices != nullptr) {
        instance->bindNVerticesVBO();
        instance->shaderPrograms.at(instance->currentShaderProgramName)
            ->enableAttribute(ShaderProgram::normalAName);
        instance->unbindVBO();
    }

    instance->bindIndicesEBO();

    glBindVertexArray(0);

    isVAOSetup = true;
}
