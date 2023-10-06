#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <array>

Values::Values(const std::array<std::optional<VertexIndexes>, 4> &values)
{
    if (!values[0].has_value() || !values[1].has_value() || !values[2].has_value())
        throw std::logic_error("Invalid argument");

    v1 = values[0].value();
    v2 = values[1].value();
    v3 = values[2].value();
    v4 = values[3];
}

Polygon::Polygon(const std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::logic_error("Could not parse value");

    storageMode = StorageMode::Static;

    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<VertexIndexes>, 4>();

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, iterEnd, ' ')))
    {
        if (i >= 4)
        {
            sValues = Values(accumulator);
            moveValuesToDynamic();
        }

        if (storageMode == StorageMode::Dynamic)
            dValues.value().push_back(VertexIndexes(*strPart));
        else
            accumulator[i] = VertexIndexes(*strPart);

        ++i;
    }

    if (storageMode == StorageMode::Static)
        sValues = Values(accumulator);

    accumulator.fill(std::nullopt);
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
