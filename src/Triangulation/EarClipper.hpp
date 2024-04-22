#pragma once

#include <Triangle.hpp>

#include <geometric.hpp>
#include <qualifier.hpp>
#include <vector_float3.hpp>

#include <cmath>
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

    static Triangle
    clipEar(std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices);

    static bool isConvexVertex(
        const glm::vec4& vertex,
        const glm::vec4& prevVertex,
        const glm::vec4& nextVertex);

public:
    static std::vector<Triangle>
    triangulate(std::vector<std::pair<glm::vec4, VertexIds>>& polygonVertices);
};
