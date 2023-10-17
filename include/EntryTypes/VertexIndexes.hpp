#pragma once

#include <optional>
#include <string>

class VertexIndexes
{
public:
    VertexIndexes();
    VertexIndexes(
        const int _vertexId,
        const std::optional<int> &_tVertexId = std::nullopt,
        const std::optional<int> &_nVertexId = std::nullopt);

    static VertexIndexes parse(const std::string &str);

    const int cGetVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};
