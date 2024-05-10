#include <ObjectBuilder.hpp>

#include <EntryIds.hpp>
#include <Globals.hpp>
#include <Hasher.hpp>
#include <IBuilder.hpp>
#include <Object.hpp>
#include <ObjectEntry.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>
#include <Triangle.hpp>

#include <qualifier.hpp>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <execution>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

TransformEntryResult::TransformEntryResult(
    const ObjectEntry& entry,
    const std::size_t id,
    const std::size_t hash,
    const std::size_t vertexId)
    : entry(entry)
    , id(id)
    , hash(hash)
    , vertexId(vertexId)
{
}

ObjectBuilder::ObjectBuilder()
    : isInited(false)
    , isTransformed(false)
    , isVAOSetup(false)
    , isTrianglesAdded(false)
    , isLinesAdded(false)
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

    vertices.reset();
    nVertices.reset();
    tVertices.reset();
    triangles.reset();
    lines.reset();
    indices.reset();

    std::cout << "HERE THREE" << std::endl;

    indicesIdToVertexId.clear();
    vericesIdToIndicesId.clear();

    isInited = false;
    isTransformed = false;
    isVAOSetup = false;
    isTrianglesAdded = false;
    isLinesAdded = true;
}

std::size_t ObjectBuilder::hashEntryIdsNaive(
    const int vertexId,
    const std::optional<int> tVertexId,
    const std::optional<int> nVertexId)
{
    std::size_t hash {};
    const auto intHasher = std::hash<int>();
    const auto charHasher = std::hash<char>();

    hash ^= intHasher(vertexId);

    if (nVertexId.has_value()) {
        hash ^= charHasher('n') << 2;
        hash ^= intHasher(*nVertexId) << 2;
    }

    if (tVertexId.has_value()) {
        hash ^= charHasher('t') << 4;
        hash ^= intHasher(*tVertexId) << 4;
    }

    return hash;
}

// TODO: Move to ObjectEntry::getHash()
std::size_t ObjectBuilder::hashEntryIdsRotl(
    const std::size_t vertexId,
    const std::optional<std::size_t> tVertexId,
    const std::optional<std::size_t> nVertexId)
{
    std::size_t hash {};

    hash = Hasher::hashCombine(hash, vertexId);

    if (nVertexId.has_value()) {
        hash = Hasher::hashCombine(hash, 'n');
        hash = Hasher::hashCombine(hash, *nVertexId);
    }

    if (tVertexId.has_value()) {
        hash = Hasher::hashCombine(hash, 't');
        hash = Hasher::hashCombine(hash, *tVertexId);
    }

    return hash;
}

void ObjectBuilder::transform()
{
    if (instance->drawMode == GL_TRIANGLES) {
        transformTriangles();
        std::cout << "HERE FOUR" << std::endl;
    } else if (instance->drawMode == GL_LINES) {
        transformLines();
    }

    isTransformed = true;
}

void ObjectBuilder::transformTriangles()
{
    throwIfNotInited("Can't transform triangles. ");

    const auto transformHashesStart = std::chrono::high_resolution_clock::now();

    std::vector<TransformEntryResult> indicesIdToTransformEntryResult;

#pragma omp parallel if (!_IS_DEBUG)
    {
        std::vector<TransformEntryResult>
            indicesIdToTransformEntryResultPrivate;

#pragma omp for nowait
        for (std::size_t triangleId = 0; triangleId < triangles->size();
             ++triangleId) {
            const auto& triangle = triangles->at(triangleId);

            for (auto i = 0; i < Triangle::verticesCount; ++i) {
                // TODO: Move to func
                // TODO: Add separate funcs for parallel and not
                const auto& entryIds = triangle.cGetVertexIds(i);

                const auto vertexId = entryIds.cGetVertexId() - 1;
                const auto& vertex = vertices->at(vertexId);

                const auto& nVertexId = entryIds.cGetNormalVertexId();
                std::optional<glm::vec3> nVertex {};
                if (nVertexId.has_value() && nVertices != nullptr) {
                    nVertex = nVertices->at(*nVertexId - 1);
                }

                const auto& tVertexId = entryIds.cGetTextureVertexId();
                std::optional<glm::vec2> tVertex {};
                if (tVertexId.has_value() && tVertices != nullptr) {
                    tVertex = tVertices->at(*tVertexId - 1);
                }

                const auto idsHash
                    = hashEntryIdsRotl(vertexId, tVertexId, nVertexId);

                indicesIdToTransformEntryResultPrivate.emplace_back(
                    ObjectEntry(vertex, nVertex, tVertex),
                    triangleId * Triangle::verticesCount + i,
                    idsHash,
                    vertexId);
            }
        }

#pragma omp critical(indicesIdToEntryWithHash)
        // /*
        std::copy(
            indicesIdToTransformEntryResultPrivate.begin(),
            indicesIdToTransformEntryResultPrivate.end(),
            std::back_inserter(indicesIdToTransformEntryResult));
        // */
    }

    std::cout << "Entries count: " << indicesIdToTransformEntryResult.size()
              << std::endl;

    const auto transformEntriesStart
        = std::chrono::high_resolution_clock::now();

    std::cout << "Transform hashes time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     transformEntriesStart - transformHashesStart)
              << std::endl;

    std::unordered_map<std::size_t, std::size_t> entriesIdsHashWithPos;
    std::vector<ObjectEntry> uniqueEntries;

    // FIXME: 17 seconds of de-alloc, shit
    // std::unordered_map<std::size_t, std::vector<std::size_t>>
    //     vericesIdToIndicesId;
    // std::unordered_map<std::size_t, std::size_t> indicesIdToVertexId;

    instance->indices.resize(triangles->size() * 3);

    std::size_t uniqueEntriesId = 0;

    // /*
    for (auto&& trEntry : indicesIdToTransformEntryResult) {
        auto& index = instance->indices.at(trEntry.id);
        const auto vertexId = trEntry.vertexId;

        const auto insertRes = entriesIdsHashWithPos.insert(
            std::make_pair(trEntry.hash, uniqueEntriesId));

        if (insertRes.second) {
            uniqueEntries.push_back(trEntry.entry);

            indicesIdToVertexId[uniqueEntriesId] = vertexId;
            vericesIdToIndicesId[vertexId].push_back(uniqueEntriesId);

            index = uniqueEntriesId++;
        } else {
            index = insertRes.first->second;
        }
    }
    // */

    std::cout << "Unique entries count: " << uniqueEntries.size() << std::endl;

    const auto transformConnectedStart
        = std::chrono::high_resolution_clock::now();

    std::cout << "Transform entries time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     transformConnectedStart - transformEntriesStart)
              << std::endl;

    // /*
#pragma parallel for if (!_IS_DEBUG)
    for (auto iter = indicesIdToVertexId.begin();
         std::distance(iter, indicesIdToVertexId.end()) > 0;
         ++iter) {
        instance->connectedIndicesIds[iter->first]
            = vericesIdToIndicesId[iter->second];
    }
    // */

    const auto transformSeparationStart
        = std::chrono::high_resolution_clock::now();

    std::cout << "Transform connected time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     transformSeparationStart - transformConnectedStart)
              << std::endl;

    instance->trVertices.resize(uniqueEntries.size() * glm::vec4::length());
    instance->trTVertices.resize(uniqueEntries.size() * glm::vec2::length());
    instance->trNVertices.resize(uniqueEntries.size() * glm::vec3::length());

#pragma omp parallel for
    for (auto iter = uniqueEntries.begin(); iter < uniqueEntries.end();
         ++iter) {
        const auto id = iter - uniqueEntries.begin();
        const auto vertex = iter->getVertex();
        const auto verticesPos = id * 4;

        instance->trVertices[verticesPos] = vertex.x;
        instance->trVertices[verticesPos + 1] = vertex.y;
        instance->trVertices[verticesPos + 2] = vertex.z;
        instance->trVertices[verticesPos + 3] = vertex.w;

        const auto nVertex = iter->getNVertex();
        const auto nVerticesPos = id * 3;

        if (nVertex.has_value()) {
            instance->trNVertices[nVerticesPos] = nVertex->x;
            instance->trNVertices[nVerticesPos + 1] = nVertex->y;
            instance->trNVertices[nVerticesPos + 2] = nVertex->z;
        }

        const auto tVertex = iter->getTVertex();
        const auto tVerticesPos = id * 2;

        if (tVertex.has_value()) {
            instance->trTVertices[tVerticesPos] = tVertex->x;
            instance->trTVertices[tVerticesPos + 1] = tVertex->y;
        }
    }

    std::cout << "Transform separation time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now()
                     - transformSeparationStart)
              << std::endl;
}

void ObjectBuilder::transformLines()
{
    throwIfNotInited("Can't transform lines. ");

    const auto transformLinesStart = std::chrono::high_resolution_clock::now();

    instance->indices.resize(lines->size() * glm::vec2::length());

    std::size_t indicesId = 0;

    std::cout << "Lines count x2: " << lines->size() * glm::vec2::length()
              << std::endl;

    for (const auto& line : *lines) {
        instance->indices[indicesId++] = line.x - 1;
        instance->indices[indicesId++] = line.y - 1;
    }

    const auto transformSeparationStart
        = std::chrono::high_resolution_clock::now();

    std::cout << "Transform lines entries time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     transformSeparationStart - transformLinesStart)
              << std::endl;

    instance->trVertices.resize(vertices->size() * glm::vec4::length());

    for (std::size_t verticesPos = 0, trVerticesPos = 0;
         verticesPos < vertices->size();
         ++verticesPos, trVerticesPos += 4) {
        const auto& vertex = vertices->at(verticesPos);

        instance->trVertices[trVerticesPos] = vertex.x;
        instance->trVertices[trVerticesPos + 1] = vertex.y;
        instance->trVertices[trVerticesPos + 2] = vertex.z;
        instance->trVertices[trVerticesPos + 3] = vertex.w;
    }

    std::cout << "Transform lines separation time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now()
                     - transformSeparationStart)
              << std::endl;
}

bool ObjectBuilder::isShaderProgramFinished() const
{
    return instance->shaderProgram != nullptr;
}

bool ObjectBuilder::isFinished() const
{
    return IBuilder<Object>::isFinished() && isInited
        && isShaderProgramFinished() && isTransformed && isVAOSetup
        && (isTrianglesAdded || isLinesAdded);
}

void ObjectBuilder::init(
    const std::string_view name,
    const GLenum drawMode,
    std::unique_ptr<std::vector<glm::vec4>> _vertices)
{
    if (isInited) {
        throw std::logic_error(
            "Can't init ObjectBuilder. It has already been initialised");
    }

    instance->name = name;
    instance->drawMode = drawMode;

    vertices = std::move(_vertices);

    glGenVertexArrays(1, &instance->VAO);
    glGenBuffers(1, &instance->verticesVBO);
    glGenBuffers(1, &instance->tVerticesVBO);
    glGenBuffers(1, &instance->nVerticesVBO);
    glGenBuffers(1, &instance->EBO);

    isInited = true;
}

void ObjectBuilder::addTriangles(
    std::unique_ptr<std::vector<Triangle>> _triangles)
{
    const std::string error = "Can't add triangles to ObjectBuilder. ";

    throwIfNotInited(error);

    if (triangles != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    triangles = std::move(_triangles);

    isTrianglesAdded = true;
}

void ObjectBuilder::addLines(
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> _lines)
{
    const std::string error = "Can't add lines to ObjectBuilder. ";

    throwIfNotInited(error);

    if (lines != nullptr) {
        throw std::logic_error(error + "They have been already added.");
    }

    lines = std::move(_lines);

    isLinesAdded = true;
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
    std::shared_ptr<ShaderProgram> shaderProgram)
{
    if (instance->shaderProgram != nullptr) {
        throw std::logic_error(
            "Can't add shader program. It has been already added");
    }

    instance->shaderProgram = std::move(shaderProgram);
}

void ObjectBuilder::addTexture(std::unique_ptr<Texture> texture)
{
    instance->textures.insert(
        std::make_pair(texture->getName(), std::move(texture)));
}

void ObjectBuilder::setupVAO()
{
    if (!isTransformed) {
        throw std::logic_error(
            "Can't setup VAO. ObjectBuilder haven't been merged.");
    }

    glBindVertexArray(instance->VAO);

    instance->bindVerticesVBO();
    instance->shaderProgram->enableAttribute(ShaderProgram::positionAName);
    instance->unbindVBO();

    if (tVertices != nullptr) {
        instance->bindTVerticesVBO();
        instance->shaderProgram->enableAttribute(ShaderProgram::texCoordAName);
        instance->unbindVBO();
    }

    if (nVertices != nullptr) {
        instance->bindNVerticesVBO();
        instance->shaderProgram->enableAttribute(ShaderProgram::normalAName);
        instance->unbindVBO();
    }

    instance->bindEBO();

    glBindVertexArray(0);

    isVAOSetup = true;
}
