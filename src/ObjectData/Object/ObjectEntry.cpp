#include <ObjectEntry.hpp>

#include <qualifier.hpp>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <functional>

ObjectEntry::ObjectEntry()
    : vertex() {};

ObjectEntry::ObjectEntry(
    const glm::vec4& _vertex,
    const std::optional<glm::vec3>& _nVertex,
    const std::optional<glm::vec2>& _tVertex)
    : vertex(_vertex)
    , nVertex(_nVertex)
    , tVertex(_tVertex)
{
}

bool ObjectEntry::operator==(const ObjectEntry& entry) const
{
    return vertex == entry.vertex && tVertex == entry.tVertex
        && nVertex == entry.nVertex;
}

[[nodiscard]] std::size_t ObjectEntry::hash() const
{
    std::size_t hash {};
    auto shift = 0;
    const auto floatHasher = std::hash<float>();

    hash ^= floatHasher(vertex.x) << shift++;
    hash ^= floatHasher(vertex.y) << shift++;
    hash ^= floatHasher(vertex.z) << shift++;
    hash ^= floatHasher(vertex.w) << shift++;

    if (nVertex.has_value()) {
        hash ^= floatHasher(nVertex->x) << shift++;
        hash ^= floatHasher(nVertex->y) << shift++;
        hash ^= floatHasher(nVertex->z) << shift++;
    }

    if (tVertex.has_value()) {
        hash ^= floatHasher(tVertex->x) << shift++;
        hash ^= floatHasher(tVertex->y) << shift++;
    }

    return hash;
}

std::size_t
ObjectEntry::HashFunction::operator()(const ObjectEntry& entry) const
{
    return entry.hash();
}

glm::vec4 ObjectEntry::getVertex() const { return vertex; }

std::optional<glm::vec3> ObjectEntry::getNVertex() const { return nVertex; }

std::optional<glm::vec2> ObjectEntry::getTVertex() const { return tVertex; }
