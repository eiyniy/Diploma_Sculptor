#pragma once

#include <Matrix.hpp>

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

class Triangle;

class Graph {
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
    std::vector<const Vector<4>*> nodes;

    [[nodiscard]] int findNearestVertexId(std::pair<int, int> pos) const;

    // TODO: Move this shit from here
    static double findDistance(std::pair<int, int> v1, std::pair<int, int> v2);

public:
    Graph();

    Graph(
        const std::vector<Vector<4>>& drawableVertices,
        const std::vector<Triangle>& polygons);

    [[nodiscard]] std::unique_ptr<std::vector<int>>
    getAffectedVerticesIds(std::pair<int, int> mousePos, int radius) const;
};

// TODO: Move to Point
inline double
Graph::findDistance(const std::pair<int, int> v1, const std::pair<int, int> v2)
{
    return std::sqrt(
        std::pow(std::fabs(v2.first - v1.first), 2)
        + std::pow(std::fabs(v2.second - v1.second), 2));
}
