#pragma once

#include <EntryIds.hpp> // IWYU pragma: keep

#include <array>
#include <vector>

class Triangle {
private:
    int vertexIndexesCount;

    std::array<EntryIds, 3> values;

public:
    Triangle(const std::vector<EntryIds>& indexes);

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const EntryIds& cGetVertexIds(int i) const;
};
