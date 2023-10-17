#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <array>

SValues::SValues(const std::vector<VertexIndexes> &values)
    : v4(std::nullopt)
{
    if (values.size() != 3 || values.size() != 4)
        throw std::logic_error("Invalid argument");

    v1 = values[0];
    v2 = values[1];
    v3 = values[2];

    if (values.size() == 4)
        v4 = values[3];
}

Polygon::Polygon(const std::vector<VertexIndexes> &indexes)
{
    vertexIndexesCount = indexes.size();

    if (vertexIndexesCount <= 4)
    {
        storageMode = StorageMode::Static;
        sValues = SValues(indexes);
    }
    else
    {
        storageMode = StorageMode::Dynamic;
        dValues = indexes;
    }
}

Polygon Polygon::parse(const std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::logic_error("Could not parse value");

    static auto accumulator = std::vector<VertexIndexes>();
    accumulator.reserve(3);

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while (auto strPart = ObjParser::getNextPart(&iter, iterEnd, ' '))
    {
        accumulator.emplace_back(VertexIndexes::parse(*strPart));
        ++i;
    }

    auto res = Polygon(accumulator);
    accumulator.clear();
    return res;
}

const int Polygon::cGetVertexIndexesCount() const
{
    return vertexIndexesCount;
}

const VertexIndexes &Polygon::cGetVertexIndexes(const int i) const
{
    switch (storageMode)
    {
    case StorageMode::Static:

        switch (i)
        {
        case 0:
            return sValues->v1;
            break;
        case 1:
            return sValues->v2;
            break;
        case 2:
            return sValues->v3;
            break;
        case 3:
            if (!sValues->v4.has_value())
                throw std::invalid_argument("Could not get VertexIndexes");

            return *sValues->v4;
            break;

        default:
            throw std::invalid_argument("Could not get VertexIndexes");
            break;
        }

        break;
    case StorageMode::Dynamic:
        return dValues->at(i);
        break;
    }
}

void Polygon::moveValuesToDynamic()
{
    if (!sValues.has_value())
        throw std::logic_error("Could not store polygon");

    storageMode = StorageMode::Dynamic;

    dValues = std::vector<VertexIndexes>(4);

    dValues->at(0) = sValues->v1;
    dValues->at(1) = sValues->v2;
    dValues->at(2) = sValues->v3;
    dValues->at(3) = *sValues->v4;
}
