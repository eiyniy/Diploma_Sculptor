#pragma once

#include <vector_float4.hpp>

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

class Triangle;

class Graph {
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
    std::vector<const glm::vec4*> nodes;

    [[nodiscard]] int findNearestVertexId(std::pair<int, int> pos) const;

    static double findDistance(std::pair<int, int> v1, std::pair<int, int> v2);

public:
    Graph();

    Graph(
        const std::vector<glm::vec4>& drawableVertices,
        const std::vector<Triangle>& polygons);

    [[nodiscard]] std::unique_ptr<std::vector<int>>
    getAffectedVerticesIds(std::pair<int, int> mousePos, int radius) const;
};
