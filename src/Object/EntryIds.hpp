#pragma once

#include <optional>
#include <string>

class EntryIds {
public:
    EntryIds();

    explicit EntryIds(
        int _vertexId,
        const std::optional<int>& _tVertexId = std::nullopt,
        const std::optional<int>& _nVertexId = std::nullopt);

    static EntryIds parse(const std::string& str);

    [[nodiscard]] int cGetVertexId() const;

    [[nodiscard]] std::optional<int> cGetNormalVertexId() const;

    [[nodiscard]] std::optional<int> cGetTextureVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};