#include <EarClipper.hpp>

#include <Triangle.hpp>

#include <qualifier.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>

#include <stdexcept>
#include <vector>

std::vector<Triangle> EarClipper::triangulate(
    std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices)
{
    std::vector<Triangle> result;

    while (polygonVertices.size() >= 3) {
        result.emplace_back(clipEar(polygonVertices));
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
    std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices)
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
            { it->second, itNext->second, itPrev->second }
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

double
EarClipper::area(const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2)
{
    const auto vec1 = v1 - v0;
    const auto vec2 = v2 - v0;

    const auto crossProduct = vec1 * vec2;
    const auto sin
        = glm::length(crossProduct) / (glm::length(vec1) * glm::length(vec2));

    return std::fabs(
        triangleToParallelogramRatio * glm::length(vec1) * glm::length(vec2)
        * sin);
}
