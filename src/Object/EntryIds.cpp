#include <EntryIds.hpp>

#include <Math.hpp>
#include <ObjParser.hpp>
#include <Triangle.hpp>

#include <array>
#include <charconv>

EntryIds::EntryIds()
    : vertexId(0)
{
}

EntryIds::EntryIds(
    const std::size_t _vertexId,
    const std::optional<std::size_t>& _tVertexId,
    const std::optional<std::size_t>& _nVertexId)
    : vertexId(_vertexId)
    , tVertexId(_tVertexId)
    , nVertexId(_nVertexId)
{
}

EntryIds EntryIds::parse(const std::string_view str)
{
    std::array<std::optional<std::size_t>, 3> entryIds;

    size_t startPos = 0;
    size_t endPos = str.find('/');

    for (auto& entryId : entryIds) {
        std::string_view entryIdStr = str.substr(startPos, endPos - startPos);

        if (!entryIdStr.empty()) {
            std::from_chars(
                entryIdStr.data(),
                entryIdStr.data() + entryIdStr.length(),
                entryId.emplace());
        } else {
            entryId = std::nullopt;
        }

        if (endPos == std::string_view::npos) {
            break;
        }

        startPos = endPos + 1;
        endPos = str.find('/', startPos);
    }

    return EntryIds { *entryIds[0], entryIds[1], entryIds[2] };
}

std::size_t EntryIds::cGetVertexId() const { return vertexId; }

const std::optional<std::size_t>& EntryIds::cGetNormalVertexId() const
{
    return nVertexId;
}

const std::optional<std::size_t>& EntryIds::cGetTextureVertexId() const
{
    return tVertexId;
}
