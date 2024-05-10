#pragma once

#include <EntryIds.hpp> // IWYU pragma: keep

#include <array>
#include <vector>

class Triangle {
private:
    std::array<EntryIds, 3> values;

public:
    static constexpr int verticesCount = 3;

    Triangle(const std::vector<EntryIds>& indexes);

    [[nodiscard]] const EntryIds& cGetVertexIds(std::size_t i) const;
};
