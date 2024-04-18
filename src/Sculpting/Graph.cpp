#include <Graph.hpp>

#include <Triangle.hpp>

#include <type_vec4.hpp>
#include <vector_float4.hpp>

#include <array>
#include <cmath>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

Graph::Graph() = default;

Graph::Graph(
    const std::vector<glm::vec4>& drawableVertices,
    const std::vector<Triangle>& polygons)
{
    const auto size = drawableVertices.size();

    nodes.reserve(size);
    adjacencyMatrix.resize(size);
    for (auto&& row : adjacencyMatrix) {
        row.resize(size, false);
    }

    for (int i = 0; i < size; ++i) {
        nodes.emplace_back(&drawableVertices[i]);
    }

    for (auto&& triangle : polygons) {
        std::array<int, 3> connectedVertices {};

        for (int i = 0; i < triangle.cGetVertexIdsCount(); ++i) {
            connectedVertices.at(i)
                = triangle.cGetVertexIds(i).cGetVertexId() - 1;
        }

        adjacencyMatrix[connectedVertices[0]][connectedVertices[1]] = true;
        adjacencyMatrix[connectedVertices[1]][connectedVertices[0]] = true;

        adjacencyMatrix[connectedVertices[1]][connectedVertices[2]] = true;
        adjacencyMatrix[connectedVertices[2]][connectedVertices[1]] = true;

        adjacencyMatrix[connectedVertices[2]][connectedVertices[0]] = true;
        adjacencyMatrix[connectedVertices[0]][connectedVertices[2]] = true;
    }
}

int Graph::findNearestVertexId(const std::pair<int, int> pos) const
{
    int id = 0;
    double distance = INFINITY;

    for (int i = 0; i < nodes.size(); ++i) {
        const std::pair<int, int> nodePos { (int)nodes[i]->x,
                                            (int)nodes[i]->y };
        const auto newDistance = findDistance(pos, nodePos);

        if (newDistance < distance) {
            distance = newDistance;
            id = i;
        }
    }

    return id;
}

std::unique_ptr<std::vector<int>> Graph::getAffectedVerticesIds(
    const std::pair<int, int> mousePos, const int radius) const
{
    auto affectedVertices = std::make_unique<std::vector<int>>();

    const auto nearestVertexId = findNearestVertexId(mousePos);

    std::vector<bool> used;
    used.resize(nodes.size(), false);

    std::queue<int> idsQueue;
    idsQueue.push(nearestVertexId);
    used[nearestVertexId] = true;

    const std::pair<int, int> nearestVertexPos {
        (int)nodes[nearestVertexId]->x, (int)nodes[nearestVertexId]->y
    };

    // std::cout << "Nearest vertex pos: " << nearestVertexPos.first << ' '
    //   << nearestVertexPos.second << std::endl;

    while (!idsQueue.empty()) {
        int id = idsQueue.front();
        idsQueue.pop();

        const std::pair<int, int> nodePos { (int)nodes[id]->x,
                                            (int)nodes[id]->y };
        const auto distance = findDistance(mousePos, nodePos);

        if (distance > radius) {
            continue;
        }

        // if (distance <= radius)
        affectedVertices->emplace_back(id);

        for (int i = 0; i < adjacencyMatrix[id].size(); ++i) {
            if (!adjacencyMatrix[id][i]) {
                continue;
            }

            if (!used[i]) {
                idsQueue.push(i);
                used[i] = true;
            }
        }
    }

    return std::move(affectedVertices);
}

// TODO: Move to Point
double
Graph::findDistance(const std::pair<int, int> v1, const std::pair<int, int> v2)
{
    return std::sqrt(
        std::pow(std::fabs(v2.first - v1.first), 2)
        + std::pow(std::fabs(v2.second - v1.second), 2));
}
