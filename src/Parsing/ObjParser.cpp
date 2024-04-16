#include <ObjParser.hpp>

#include <BaseTextParser.hpp>
#include <Enums.hpp>
#include <Globals.hpp>
#include <MtlParser.hpp>
#include <OldObject.hpp>
#include <Triangle.hpp>

#include <qualifier.hpp>
#include <type_vec4.hpp>
#include <vector_float4.hpp>

#include <chrono>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

ObjParser::ObjParser(const std::string& _pathToObj)
    : BaseTextParser(_pathToObj)
{
}

std::optional<ObjEntryType> ObjParser::getEntryType(const std::string& line)
{
    auto iter = line.begin();

    auto type = getNextPart(&iter, line.end(), ' ');

    if (type && type == "mtllib") {
        return ObjEntryType::MtlPath;
    }
    if (type && type == "usemtl") {
        return ObjEntryType::UseMtl;
    }
    if (type && type == "v") {
        return ObjEntryType::Vertex;
    }
    if (type && type == "vt") {
        return ObjEntryType::TextureVertex;
    }
    if (type && type == "vn") {
        return ObjEntryType::NormalVertex;
    }
    if (type && type == "f") {
        return ObjEntryType::Polygon;
    }
    return {};
}

OldObject* ObjParser::parse()
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    const auto fileContent = readFile();
    const auto lines = splitByLines(fileContent);

    for (const auto& line : lines) {
        parseEntry(line);
    }

    polygons.reserve(polygonsStringAndMaterial.size());

#pragma omp parallel for if (!_IS_DEBUG)
    for (const auto& pair : polygonsStringAndMaterial) {
        const auto triangulated
            = Triangle::parseAndTriangulate(pair.first, vertices, pair.second);
        for (const auto& polygon : triangulated) {
#pragma omp critical(new_triangle)
            polygons.emplace_back(polygon);
        }
    }

    const auto timeEnd = std::chrono::high_resolution_clock::now();
    auto parseTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         timeEnd - timeStart)
                         .count();
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    return new OldObject(
        vertices, tVertices, nVertices, polygons, std::move(materials));
}

void ObjParser::parseEntry(const std::string& line)
{
    const auto type = getEntryType(line);
    if (!type.has_value()) {
        return;
    }

    std::array<std::optional<double>, 4> acc;
    if (type != ObjEntryType::Polygon && type != ObjEntryType::MtlPath
        && type != ObjEntryType::UseMtl) {
        acc = parseAcc(line);
    }

    switch (*type) {
    case ObjEntryType::MtlPath: {
        auto iter = line.cbegin();
        const auto iterEnd = line.cend();

        ObjParser::getNextPart(&iter, iterEnd, ' ');
        const auto optMtlName = ObjParser::getNextPart(&iter, line.end(), ' ');

        if (!optMtlName) {
            throw std::runtime_error("Can't parse .mtl file name");
        }

        mtlPath = getPathToFile().substr(0, getPathToFile().rfind('/') + 1)
            + *optMtlName;

        MtlParser mtlParser { mtlPath };
        materials = std::move(mtlParser.parse());
        break;
    }
    case ObjEntryType::UseMtl: {
        auto iter = line.cbegin();
        const auto iterEnd = line.cend();

        ObjParser::getNextPart(&iter, iterEnd, ' ');
        const auto optCurrMaterialName
            = ObjParser::getNextPart(&iter, line.end(), ' ');

        if (!optCurrMaterialName) {
            throw std::runtime_error("Can't parse usemtl name");
        }

        currMaterialName = *optCurrMaterialName;
        break;
    }
    case ObjEntryType::Polygon:
        polygonsStringAndMaterial.emplace_back(line, currMaterialName);
        break;
    case ObjEntryType::Vertex:
        vertices.emplace_back(parseVertex(acc));
        break;
    case ObjEntryType::NormalVertex:
        nVertices.emplace_back(parseNVertex(acc));
        break;
    case ObjEntryType::TextureVertex:
        tVertices.emplace_back(parseTVertex(acc));
        break;
    }
}

std::array<std::optional<double>, 4>
ObjParser::parseAcc(const std::string& line)
{
    const auto entryType = ObjParser::getEntryType(line);
    if (entryType != ObjEntryType::Vertex
        && entryType != ObjEntryType::TextureVertex
        && entryType != ObjEntryType::NormalVertex) {
        throw std::logic_error("Could not parse value");
    }

    std::optional<std::string> strPart;
    auto accumulator = std::array<std::optional<double>, 4>();

    auto iter = line.cbegin();
    const auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    for (; (strPart = ObjParser::getNextPart(&iter, line.end(), ' ')); ++i) {
        accumulator.at(i) = std::stod(*strPart);
    }

    if (i < 1) {
        throw std::logic_error("ObjParser. Can't parse value");
    }

    return accumulator;
}

glm::vec4
ObjParser::parseVertex(const std::array<std::optional<double>, 4>& acc)
{
    return { *acc[0], *acc[1], *acc[2], acc[3].value_or(1) };
}

glm::vec4
ObjParser::parseNVertex(const std::array<std::optional<double>, 4>& acc)
{
    return { *acc[0], *acc[1], *acc[2], 0 };
}

glm::vec4
ObjParser::parseTVertex(const std::array<std::optional<double>, 4>& acc)
{
    return { *acc[0], acc[1].value_or(0), acc[2].value_or(0), 1 };
}
