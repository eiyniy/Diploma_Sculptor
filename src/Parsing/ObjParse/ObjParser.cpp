#include <ObjParser.hpp>

#include <BaseTextParser.hpp>
#include <EarClipper.hpp>
#include <Enums.hpp>
#include <ObjParseResult.hpp>
#include <Triangle.hpp>

#include <qualifier.hpp>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

ObjParser::ObjParser(const std::string& _pathToObj)
    : BaseTextParser(_pathToObj)
{
    vertices = std::make_unique<std::vector<glm::vec4>>();
    nVertices = std::make_unique<std::vector<glm::vec3>>();
    tVertices = std::make_unique<std::vector<glm::vec2>>();
    polygons = std::make_unique<std::vector<Triangle>>();
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

ObjParseResult ObjParser::parse()
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    const auto fileContent = readFile();
    const auto lines = splitByLines(fileContent);

    for (const auto& line : lines) {
        parseEntry(line);
    }

    const auto timeVertices = std::chrono::high_resolution_clock::now();
    auto verticeParseTime
        = std::chrono::duration_cast<std::chrono::milliseconds>(
              timeVertices - timeStart)
              .count();
    std::cout << "vertices parse time: " << verticeParseTime << " ms"
              << std::endl;

    polygons->reserve(polygonStrings.size());

    for (const auto& string : polygonStrings) {
        for (const auto& polygon : parsePolygon(string)) {
            polygons->emplace_back(polygon);
        }
    }

    const auto timeEnd = std::chrono::high_resolution_clock::now();
    auto trianglesParseTime
        = std::chrono::duration_cast<std::chrono::milliseconds>(
              timeEnd - timeVertices)
              .count();
    auto parseTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         timeEnd - timeStart)
                         .count();
    std::cout << "Triangles parse time: " << trianglesParseTime << " ms"
              << std::endl;
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    return { std::move(vertices),
             std::move(nVertices),
             std::move(tVertices),
             std::move(polygons) };
}

void ObjParser::parseEntry(const std::string& line)
{
    const auto type = getEntryType(line);
    if (!type.has_value()) {
        return;
    }

    std::size_t pos = 0;
    std::size_t end = 0;

    switch (*type) {
    case ObjEntryType::Vertex: {
        vertices->push_back(parseGlmVec<glm::vec4>(line, fillVec4));
        break;
    }
    case ObjEntryType::NormalVertex: {
        nVertices->push_back(parseGlmVec<glm::vec3>(line, fillVec3));
        break;
    }
    case ObjEntryType::TextureVertex: {
        tVertices->push_back(parseGlmVec<glm::vec2>(line, fillVec2));
        break;
    }
    case ObjEntryType::Polygon: {
        polygonStrings.push_back(line);
        break;
    }
    case ObjEntryType::MtlPath:
    case ObjEntryType::UseMtl:
        break;
    }
}

void ObjParser::fillVec4(glm::vec4& vec, char index, float value)
{
    if (index == 0) {
        vec.x = value;
    } else if (index == 1) {
        vec.y = value;
    } else if (index == 2) {
        vec.z = value;
    } else if (index == 3) {
        vec.w = value;
    }
}

void ObjParser::fillVec3(glm::vec3& vec, char index, float value)
{
    if (index == 0) {
        vec.x = value;
    } else if (index == 1) {
        vec.y = value;
    } else if (index == 2) {
        vec.z = value;
    }
}

void ObjParser::fillVec2(glm::vec2& vec, char index, float value)
{
    if (index == 0) {
        vec.x = value;
    } else if (index == 1) {
        vec.y = value;
    }
}

std::vector<EntryIds> ObjParser::parseVertexIds(const std::string& line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != ObjEntryType::Polygon) {
        throw std::logic_error("Could not parse polygon");
    }

    std::vector<EntryIds> accumulator {};
    accumulator.reserve(3);

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while (auto strPart = ObjParser::getNextPart(&iter, iterEnd, ' ')) {
        accumulator.emplace_back(EntryIds::parse(*strPart));
        ++i;
    }

    return accumulator;
}

std::vector<Triangle> ObjParser::parsePolygon(const std::string& line)
{
    const auto vertexIds = parseVertexIds(line);

    std::vector<std::pair<glm::vec4, EntryIds>> polygonVertices;

    const auto polygonSize = vertexIds.size();
    for (int i = 0; i != polygonSize; ++i) {
        polygonVertices.emplace_back(vertices->at(i), vertexIds.at(i));
    }

    return EarClipper::triangulate(polygonVertices);
}
