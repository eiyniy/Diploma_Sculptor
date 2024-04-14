#pragma once

#include <Triangle.hpp>

#include <geometric.hpp>
#include <vec4.hpp>

#include <cmath>
#include <optional>
#include <string>
#include <utility>
#include <vector>

const float triangleToParallelogramRatio = 0.5F;

class EarClipper {
private:
    static double
    area(const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2);

    static bool isPointsInside(
        const std::pair<glm::vec4, VertexIds>& v0,
        const std::pair<glm::vec4, VertexIds>& v1,
        const std::pair<glm::vec4, VertexIds>& v2,
        const std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices);

    static Triangle clipEar(
        std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices,
        const std::optional<std::string>& materialName);

    static bool isConvexVertex(
        const glm::vec4& vertex,
        const glm::vec4& prevVertex,
        const glm::vec4& nextVertex);

public:
    static std::vector<Triangle> triangulate(
        std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices,
        const std::optional<std::string>& materialName);

    static std::vector<Triangle> triangulate(
        const std::vector<VertexIds>& indexes,
        const std::vector<glm::vec4>& vertices,
        const std::optional<std::string>& materialName);
};

inline double
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
