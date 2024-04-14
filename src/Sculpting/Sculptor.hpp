#pragma once

#include <Graph.hpp>

#include <vec4.hpp>

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
        std::vector<glm::vec4>& vertices,
        std::pair<int, int> mousePos,
        const glm::vec4& direction);

    static void
    pull(OldObject* object, int triangleId, std::pair<int, int> mousePos);
};

inline int Sculptor::cGetRadius() const { return radius; }
