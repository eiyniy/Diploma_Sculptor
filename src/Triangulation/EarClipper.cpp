#include <EarClipper.hpp>

#include <Triangle.hpp>

#include <geometric.hpp>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<Triangle> EarClipper::triangulate(
    const std::vector<VertexIds>& indexes,
    const std::vector<glm::vec4>& vertices,
    const std::optional<std::string>& materialName)
{
    std::vector<std::pair<glm::vec4, VertexIds>> polygonVertices;

    const int polygonSize = (int)indexes.size();
    for (int i = 0; i != polygonSize; ++i) {
        polygonVertices.emplace_back(vertices.at(i), indexes.at(i));
    }

    return triangulate(polygonVertices, materialName);
}

std::vector<Triangle> EarClipper::triangulate(
    std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices,
    const std::optional<std::string>& materialName)
{
    std::vector<Triangle> result;

    while (polygonVertices.size() >= 3) {
        result.emplace_back(clipEar(polygonVertices, materialName));
    }

    return result;
}

bool EarClipper::isConvexVertex(
    const glm::vec4& vertex,
    const glm::vec4& prevVertex,
    const glm::vec4& nextVertex)
{
    const auto v1 = prevVertex - vertex;
    const auto v2 = nextVertex - vertex;

    const auto crossProduct = v1 * v2;
    const auto sin
        = glm::length(crossProduct) / (glm::length(v1) * glm::length(v2));

    return sin > 0;
}

Triangle EarClipper::clipEar(
    std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices,
    const std::optional<std::string>& materialName)
{
    int i = 0;
    const int size = (int)polygonVertices.size();

    for (auto it = polygonVertices.cbegin(); it != polygonVertices.cend();
         ++it, ++i) {
        auto itPrev = it;
        if (i > 0) {
            --itPrev;
        } else {
            itPrev = polygonVertices.cend() - 1;
        }

        auto itNext = it;
        if (size - i > 1) {
            ++itNext;
        } else {
            itNext = polygonVertices.cbegin();
        }

        if (!isConvexVertex(it->first, itPrev->first, itNext->first)) {
            continue;
        }

        if (isPointsInside(*it, *itPrev, *itNext, polygonVertices)) {
            continue;
        }

        const Triangle result {
            { it->second, itNext->second, itPrev->second },
            materialName
        };

        polygonVertices.erase(it);

        return result;
    }

    throw std::runtime_error("Can't clip ear.");
}

bool EarClipper::isPointsInside(
    const std::pair<glm::vec4, VertexIds>& v0,
    const std::pair<glm::vec4, VertexIds>& v1,
    const std::pair<glm::vec4, VertexIds>& v2,
    const std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices)
{
    bool result = false;

    const auto bigArea = area(v0.first, v1.first, v2.first);

    for (const auto& vertex : polygonVertices) {
        if (vertex.second.cGetVertexId() == v0.second.cGetVertexId()
            || vertex.second.cGetVertexId() == v1.second.cGetVertexId()
            || vertex.second.cGetVertexId() == v2.second.cGetVertexId()) {
            continue;
        }

        const auto area1 = area(vertex.first, v1.first, v2.first);
        const auto area2 = area(v0.first, vertex.first, v2.first);
        const auto area3 = area(v0.first, v1.first, vertex.first);

        result |= bigArea == area1 + area2 + area3;

        if (result) {
            break;
        }
    }

    return result;
}
