#pragma once

#include <cstddef>
#include <optional>
#include <string_view>

class EntryIds {
private:
    std::size_t vertexId;
    std::optional<std::size_t> tVertexId;
    std::optional<std::size_t> nVertexId;

public:
    EntryIds();

    explicit EntryIds(
        std::size_t _vertexId,
        const std::optional<std::size_t>& _tVertexId = std::nullopt,
        const std::optional<std::size_t>& _nVertexId = std::nullopt);

    static EntryIds parse(std::string_view str);

    [[nodiscard]] std::size_t cGetVertexId() const;

    [[nodiscard]] const std::optional<std::size_t>& cGetNormalVertexId() const;

    [[nodiscard]] const std::optional<std::size_t>& cGetTextureVertexId() const;
};