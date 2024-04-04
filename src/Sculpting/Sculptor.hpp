#pragma once

#include <Graph.hpp>
#include <Matrix.hpp>

#include <vector>
#include <utility>

class Object;

class Sculptor
{
private:
    int radius;
    Graph graph;

public:
    Sculptor(const int _radius);

    inline int cGetRadius() const;

    void createGraph(Object *object);

    void pull(
        std::vector<Vector<4>> &vertices,
        const std::pair<int, int> &mousePos,
        const Vector<4> &direction);

    void pull(
        Object *object,
        const int triangleId,
        const std::pair<int, int> mousePos);
};

inline int Sculptor::cGetRadius() const
{
    return radius;
}
