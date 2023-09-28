#pragma once

#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <vector>

class ObjInfo
{
public:
    void addVertex(const Vertex &vertex);
    void addTVertex(const TextureVertex &tVertex);
    void addNVertex(const NormalVertex &nVertex);
    void addPolygon(const Polygon &polygon);

    std::vector<Vertex> &getVertices();
    std::vector<TextureVertex> &getTVertices();
    std::vector<NormalVertex> &getNVertices();
    std::vector<Polygon> &getPolygons();

private:
    std::vector<Vertex> vertices;
    std::vector<TextureVertex> tVertices;
    std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;
};
