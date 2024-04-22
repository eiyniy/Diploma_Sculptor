#include <VertexIds.hpp>

#include <Math.hpp>
#include <ObjParser.hpp>

#include <stdexcept>

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

const VertexIds& Triangle::cGetVertexIds(const int i) const
{
    if (i > 2) {
        throw std::invalid_argument("Could not get VertexIds");
    }

    return values.at(i);
}

int VertexIds::cGetVertexId() const { return vertexId; }

std::optional<int> VertexIds::cGetNormalVertexId() const { return nVertexId; }

std::optional<int> VertexIds::cGetTextureVertexId() const { return tVertexId; }
