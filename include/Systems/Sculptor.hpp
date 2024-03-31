#pragma once

#include <Graph.hpp>
#include <Matrix.hpp>

#include <vector>

class Point;
class Object;

class Sculptor
{
private:
    int radius;
    Graph graph;

    // sf::CircleShape circle;

public:
    Sculptor(const int _radius);

    inline int cGetRadius() const;
    // inline sf::CircleShape &getCircle();

    void createGraph(Object *object);

    void pull(
        std::vector<Vector<4>> &vertices,
        const Point &mousePos,
        const Vector<4> &direction);

    void pull(
        Object *object,
        const int triangleId,
        const Point mousePos);
};

inline int Sculptor::cGetRadius() const
{
    return radius;
}

/*
inline sf::CircleShape &Sculptor::getCircle()
{
    return circle;
}
*/
