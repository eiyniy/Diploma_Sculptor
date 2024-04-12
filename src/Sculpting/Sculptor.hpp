#pragma once

#include <Graph.hpp>
#include <Matrix.hpp>

#include <utility>
#include <vector>

class OldObject;

class Sculptor {
private:
    int radius;
    Graph graph;

public:
    Sculptor(int _radius);

    [[nodiscard]] inline int cGetRadius() const;

    void createGraph(OldObject* object);

    void pull(
        std::vector<Vector<4>>& vertices,
        std::pair<int, int> mousePos,
        const Vector<4>& direction);

    static void
    pull(OldObject* object, int triangleId, std::pair<int, int> mousePos);
};

inline int Sculptor::cGetRadius() const { return radius; }
