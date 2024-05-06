#include <Triangle.hpp>

#include <EntryIds.hpp>

#include <vector>

Triangle::Triangle(const std::vector<EntryIds>& indexes)
    : vertexIndexesCount((int)indexes.size())
    , values { indexes[0], indexes[1], indexes[2] }
{
}

int Triangle::cGetVertexIdsCount() const { return vertexIndexesCount; }
