#pragma once

#include <vector_float4.hpp>

#include <array>
#include <optional>
#include <string>
#include <vector>

class VertexIds {
public:
    VertexIds();

    explicit VertexIds(
        int _vertexId,
        const std::optional<int>& _tVertexId = std::nullopt,
        const std::optional<int>& _nVertexId = std::nullopt);

    static VertexIds parse(const std::string& str);

    [[nodiscard]] int cGetVertexId() const;

    [[nodiscard]] std::optional<int> cGetNormalVertexId() const;

    [[nodiscard]] std::optional<int> cGetTextureVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};

inline int VertexIds::cGetVertexId() const { return vertexId; }

inline std::optional<int> VertexIds::cGetNormalVertexId() const
{
    return nVertexId;
}

inline std::optional<int> VertexIds::cGetTextureVertexId() const
{
    return tVertexId;
}

class Triangle {
public:
    Triangle(
        const std::vector<VertexIds>& indexes,
        const std::optional<std::string>& _materialName);

    static std::vector<Triangle> parseAndTriangulate(
        const std::string& line,
        const std::vector<glm::vec4>& vertices,
        const std::optional<std::string>& materialName);

    const glm::vec4& getFlatNormal(const std::vector<glm::vec4>& vertices);
    [[nodiscard]] glm::vec4 getPhongNormal(
        const std::vector<glm::vec4>& nVertices,
        float b0,
        float b1,
        float b2) const;

    const glm::vec4& getCenter(const std::vector<glm::vec4>& vertices);

    [[nodiscard]] const std::optional<std::string>& cGetMaterialName() const;

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds& cGetVertexIds(int i) const;

private:
    int vertexIndexesCount;

    std::array<VertexIds, 3> values;

    std::optional<std::string> materialName;
    // TODO: use static default material with grey color and etc.

    std::optional<glm::vec4> normal;
    std::optional<glm::vec4> center;

    static std::vector<VertexIds> parseInner(const std::string& line);
};

inline const std::optional<std::string>& Triangle::cGetMaterialName() const
{
    return materialName;
}