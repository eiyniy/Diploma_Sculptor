#pragma once

#include <VertexIndexes.hpp>
#include <Types.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>

struct Values
{
    Values(const std::array<std::optional<VertexIndexes>, 4> &values);

    VertexIndexes v1;
    VertexIndexes v2;
    VertexIndexes v3;
    std::optional<VertexIndexes> v4;
};

class Polygon
{
public:
    Polygon(const std::string &line);

private:
    StorageMode storageMode;

    std::optional<std::vector<VertexIndexes>> dValues;
    std::optional<Values> sValues;

    void moveValuesToDynamic();
};
