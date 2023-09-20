#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <string.h>

using namespace std;

Values::Values(std::vector<std::optional<VertexIndexes>> &values)
{
    if (!values.at(0).has_value() || !values.at(1).has_value() || !values.at(2).has_value())
        throw invalid_argument("Invalid argument");

    v1 = values[0].value();
    v2 = values[1].value();
    v3 = values[2].value();
    v4 = values[3];
}

Polygon::Polygon(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::invalid_argument("Could not parse value");

    storageMode = PolygonStorageMode::Static;

    std::optional<std::string> strPart;
    auto indexesAcc = vector<optional<VertexIndexes>>();
    indexesAcc.reserve(4);

    auto iter = line.begin();
    auto iterEnd = line.end();

    ObjParser::moveToNext(&iter, iterEnd, ' ');

    int i = 0;
    while (strPart = ObjParser::getNextPart(iter, iterEnd, ' '))
    {
        if (i >= 4)
            moveValuesToDynamic();

        if (storageMode == PolygonStorageMode::Dynamic)
            dValues.value().push_back(VertexIndexes(strPart.value()));
        else
            indexesAcc.push_back(VertexIndexes(strPart.value()));

        ObjParser::moveToNext(&iter, iterEnd, ' ');
        i++;
    }

    if (storageMode == PolygonStorageMode::Static)
        sValues = Values(indexesAcc);
}

void Polygon::moveValuesToDynamic()
{
    if (!sValues.has_value())
        throw runtime_error("Could not store polygon");

    storageMode = PolygonStorageMode::Dynamic;

    dValues = make_optional(vector<VertexIndexes>());
    dValues.value().reserve(5);

    dValues.value().at(0) = sValues.value().v1;
    dValues.value().at(1) = sValues.value().v2;
    dValues.value().at(2) = sValues.value().v3;
    dValues.value().at(3) = sValues.value().v4.value();
}
