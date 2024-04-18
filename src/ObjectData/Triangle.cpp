#include <Triangle.hpp>

#include <EarClipper.hpp>
#include <Enums.hpp>
#include <Math.hpp>
#include <ObjParser.hpp>

#include <geometric.hpp>
#include <qualifier.hpp>
#include <type_vec4.hpp>
#include <vector_float4.hpp>

#include <array>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

VertexIds::VertexIds(
    const int _vertexId,
    const std::optional<int>& _tVertexId,
    const std::optional<int>& _nVertexId)
    : vertexId(_vertexId)
    , tVertexId(_tVertexId)
    , nVertexId(_nVertexId)
{
}

VertexIds VertexIds::parse(const std::string& str)
{
    auto accumulator = std::array<std::optional<int>, 3>();

    auto iter = str.cbegin();
    auto iterEnd = str.cend();

    int i = 0;
    while (auto strPart = ObjParser::getNextPart(&iter, iterEnd, '/', true)) {
        accumulator.at(i) = Math::optStoi(*strPart);
        ++i;
    }

    if (!accumulator[0].has_value()) {
        throw std::logic_error("Invalid argument");
    }

    return VertexIds { *accumulator[0], accumulator[1], accumulator[2] };
}

Triangle::Triangle(
    const std::vector<VertexIds>& indexes,
    const std::optional<std::string>& _materialName)
    : vertexIndexesCount((int)indexes.size())
    , materialName(_materialName)
    , values { indexes[0], indexes[1], indexes[2] }
{
}

int Triangle::cGetVertexIdsCount() const { return vertexIndexesCount; }

const VertexIds& Triangle::cGetVertexIds(const int i) const
{
    if (i > 2) {
        throw std::invalid_argument("Could not get VertexIds");
    }

    return values.at(i);
}

std::vector<Triangle> Triangle::parseAndTriangulate(
    const std::string& line,
    const std::vector<glm::vec4>& vertices,
    const std::optional<std::string>& materialName)
{
    const auto accumulator = parseInner(line);
    return EarClipper::triangulate(accumulator, vertices, materialName);
}

const glm::vec4& Triangle::getFlatNormal(const std::vector<glm::vec4>& vertices)
{
    if (!normal.has_value()) {
        const auto& a = vertices.at(cGetVertexIds(0).cGetVertexId() - 1);
        const auto& b = vertices.at(cGetVertexIds(1).cGetVertexId() - 1);
        const auto& c = vertices.at(cGetVertexIds(2).cGetVertexId() - 1);

        const auto v0 = b - a;
        const auto v1 = c - a;

        normal = v0 * v1;
        normal = glm::normalize(*normal);
    }

    return *normal;
}

glm::vec4 Triangle::getPhongNormal(
    const std::vector<glm::vec4>& nVertices,
    const float b0,
    const float b1,
    const float b2) const
{
    glm::vec4 phongNormal;

    const auto nId0 = cGetVertexIds(0).cGetNormalVertexId();
    const auto nId1 = cGetVertexIds(1).cGetNormalVertexId();
    const auto nId2 = cGetVertexIds(2).cGetNormalVertexId();

    if (!nId0.has_value() || !nId1.has_value() || !nId2.has_value()) {
        throw std::runtime_error("Can not get normal");
    }

    const auto& aNormal = nVertices.at(*nId0 - 1);
    const auto& bNormal = nVertices.at(*nId1 - 1);
    const auto& cNormal = nVertices.at(*nId2 - 1);

    phongNormal = aNormal * b0 + bNormal * b1 + cNormal * b2;
    phongNormal = glm::normalize(phongNormal);

    return phongNormal;
}

const glm::vec4& Triangle::getCenter(const std::vector<glm::vec4>& vertices)
{
    // if (!center.has_value())
    // {
    const auto& a = vertices.at(cGetVertexIds(0).cGetVertexId() - 1);
    const auto& b = vertices.at(cGetVertexIds(1).cGetVertexId() - 1);
    const auto& c = vertices.at(cGetVertexIds(2).cGetVertexId() - 1);

    center = {
        (a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3, 1.F
    };
    // }

    return *center;
}

std::vector<VertexIds> Triangle::parseInner(const std::string& line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != ObjEntryType::Polygon) {
        throw std::logic_error("Could not parse polygon");
    }

    std::vector<VertexIds> accumulator {};
    accumulator.reserve(3);

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while (auto strPart = ObjParser::getNextPart(&iter, iterEnd, ' ')) {
        accumulator.emplace_back(VertexIds::parse(*strPart));
        ++i;
    }

    return accumulator;
}

int VertexIds::cGetVertexId() const { return vertexId; }

std::optional<int> VertexIds::cGetNormalVertexId() const { return nVertexId; }

std::optional<int> VertexIds::cGetTextureVertexId() const { return tVertexId; }

const std::optional<std::string>& Triangle::cGetMaterialName() const
{
    return materialName;
}
