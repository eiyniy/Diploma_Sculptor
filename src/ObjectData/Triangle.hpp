#pragma once

#include <VertexIds.hpp>

#include <vector_float4.hpp>

class Triangle {
private:
    int vertexIndexesCount;

    std::array<VertexIds, 3> values;

public:
    Triangle(const std::vector<VertexIds>& indexes);

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds& cGetVertexIds(int i) const;
};
