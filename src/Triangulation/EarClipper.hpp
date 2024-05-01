#pragma once

#include <Triangle.hpp>

#include <vector_float4.hpp>

#include <utility>
#include <vector>

class EntryIds;

class EarClipper {
private:
    static constexpr float triangleToParallelogramRatio = 0.5F;

    static double
    area(const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2);

    static bool isPointsInside(
        const std::pair<glm::vec4, EntryIds>& v0,
        const std::pair<glm::vec4, EntryIds>& v1,
        const std::pair<glm::vec4, EntryIds>& v2,
        const std::vector<std::pair<glm::vec4, EntryIds>>& polygonVertices);

    static Triangle
    clipEar(std::vector<std::pair<glm::vec4, EntryIds>>& polygonVertices);

    static bool isConvexVertex(
        const glm::vec4& vertex,
        const glm::vec4& prevVertex,
        const glm::vec4& nextVertex);

public:
    static std::vector<Triangle>
    triangulate(std::vector<std::pair<glm::vec4, EntryIds>>& polygonVertices);
};
