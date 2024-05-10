#include <ObjParser.hpp>

#include <BaseTextParser.hpp>
#include <EarClipper.hpp>
#include <EntryIds.hpp>
#include <Enums.hpp>
#include <Globals.hpp>
#include <ObjParseResult.hpp>
#include <Triangle.hpp>

#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <omp.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <execution>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std::string_view_literals;

ObjParser::ObjParser()
    : BaseTextParser()
{
    reset();
}

std::optional<ObjEntryType> ObjParser::getEntryType(const std::string_view line)
{
    size_t end = line.find_first_of(" \t\n\r\f\v");
    std::string_view type = line.substr(0, end);

    if (type == "mtllib"sv) {
        return ObjEntryType::MtlPath;
    }
    if (type == "usemtl"sv) {
        return ObjEntryType::UseMtl;
    }
    if (type == "v"sv) {
        return ObjEntryType::Vertex;
    }
    if (type == "vt"sv) {
        return ObjEntryType::TextureVertex;
    }
    if (type == "vn"sv) {
        return ObjEntryType::NormalVertex;
    }
    if (type == "f"sv) {
        return ObjEntryType::Polygon;
    }
    if (type == "l"sv) {
        return ObjEntryType::Line;
    }

    return std::nullopt;
}

void ObjParser::reset()
{
    vertices = std::make_unique<std::vector<glm::vec4>>();
    nVertices = std::make_unique<std::vector<glm::vec3>>();
    tVertices = std::make_unique<std::vector<glm::vec2>>();

    polygons = std::make_unique<std::vector<Triangle>>();
    linesId = std::make_unique<std::vector<glm::vec<2, GLuint>>>();

    polygonStrings.clear();
    lineStrings.clear();
}

void ObjParser::parseEntriesPar(const std::vector<std::string_view>& fileLines)
{
    std::vector<std::pair<glm::vec4, std::size_t>> verticesWithId;
    std::vector<std::pair<glm::vec3, std::size_t>> nVerticesWithId;
    std::vector<std::pair<glm::vec2, std::size_t>> tVerticesWithId;

#pragma omp parallel /*if (!_IS_DEBUG)*/
    {
        std::vector<std::pair<glm::vec4, std::size_t>> verticesPrivate;
        std::vector<std::pair<glm::vec3, std::size_t>> nVerticesPrivate;
        std::vector<std::pair<glm::vec2, std::size_t>> tVerticesPrivate;
        std::vector<std::string_view> polygonStringsPrivate;
        std::vector<std::string_view> lineStringsPrivate;

#pragma omp for
        for (std::size_t id = 0; id < fileLines.size(); ++id) {
            const auto& line = fileLines.at(id);
            const auto type = getEntryType(line);
            if (!type.has_value()) {
                continue;
            }

            switch (*type) {
            case ObjEntryType::Vertex:
                verticesPrivate.emplace_back(parseGlmVec<4, float>(line), id);
                break;
            case ObjEntryType::NormalVertex:
                nVerticesPrivate.emplace_back(parseGlmVec<3, float>(line), id);
                break;
            case ObjEntryType::TextureVertex:
                tVerticesPrivate.emplace_back(parseGlmVec<2, float>(line), id);
                break;
            case ObjEntryType::Polygon:
                polygonStringsPrivate.push_back(line);
                break;
            case ObjEntryType::Line:
                lineStringsPrivate.push_back(line);
                break;
            case ObjEntryType::MtlPath:
            case ObjEntryType::UseMtl:
                break;
            }
        }

#pragma omp critical(verticesPrivate)
        {
            std::copy(
                verticesPrivate.begin(),
                verticesPrivate.end(),
                std::back_inserter(verticesWithId));
        }

#pragma omp critical(nVerticesPrivate)
        {
            std::copy(
                nVerticesPrivate.begin(),
                nVerticesPrivate.end(),
                std::back_inserter(nVerticesWithId));
        }

#pragma omp critical(tVerticesPrivate)
        {
            std::copy(
                tVerticesPrivate.begin(),
                tVerticesPrivate.end(),
                std::back_inserter(tVerticesWithId));
        }

#pragma omp critical(polygonStringsPrivate)
        {
            std::copy(
                polygonStringsPrivate.begin(),
                polygonStringsPrivate.end(),
                std::back_inserter(polygonStrings));
        }

#pragma omp critical(lineStringsPrivate)
        {
            std::copy(
                lineStringsPrivate.begin(),
                lineStringsPrivate.end(),
                std::back_inserter(lineStrings));
        }
    }

    std::sort(
        std::execution::par,
        verticesWithId.begin(),
        verticesWithId.end(),
        [](auto&& item1, auto&& item2) { return item1.second < item2.second; });

    std::sort(
        std::execution::par,
        nVerticesWithId.begin(),
        nVerticesWithId.end(),
        [](auto&& item1, auto&& item2) { return item1.second < item2.second; });

    std::sort(
        std::execution::par,
        tVerticesWithId.begin(),
        tVerticesWithId.end(),
        [](auto&& item1, auto&& item2) { return item1.second < item2.second; });

    vertices->resize(verticesWithId.size());
    nVertices->resize(nVerticesWithId.size());
    tVertices->resize(tVerticesWithId.size());

    std::transform(
        std::execution::par,
        verticesWithId.cbegin(),
        verticesWithId.cend(),
        vertices->begin(),
        [](auto&& item) { return item.first; });

    std::transform(
        std::execution::par,
        nVerticesWithId.cbegin(),
        nVerticesWithId.cend(),
        nVertices->begin(),
        [](auto&& item) { return item.first; });

    std::transform(
        std::execution::par,
        tVerticesWithId.cbegin(),
        tVerticesWithId.cend(),
        tVertices->begin(),
        [](auto&& item) { return item.first; });
}

void ObjParser::parseEntriesSeq(const std::vector<std::string_view>& fileLines)
{
    for (const auto& line : fileLines) {
        const auto type = getEntryType(line);
        if (!type.has_value()) {
            continue;
        }

        switch (*type) {
        case ObjEntryType::Vertex:
            vertices->push_back(parseGlmVec<4, float>(line));
            break;
        case ObjEntryType::NormalVertex:
            nVertices->push_back(parseGlmVec<3, float>(line));
            break;
        case ObjEntryType::TextureVertex:
            tVertices->push_back(parseGlmVec<2, float>(line));
            break;
        case ObjEntryType::Polygon:
            polygonStrings.push_back(line);
            break;
        case ObjEntryType::Line:
            lineStrings.push_back(line);
            break;
        case ObjEntryType::MtlPath:
        case ObjEntryType::UseMtl:
            break;
        }
    }
}

ObjParseResult ObjParser::parse(const std::string& pathToObj)
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    const auto fileContent = readFile(pathToObj);
    const auto fileLines = splitByLines(fileContent);

    parseEntriesSeq(fileLines);

    const auto timeVertices = std::chrono::high_resolution_clock::now();
    auto verticeParseTime
        = std::chrono::duration_cast<std::chrono::milliseconds>(
              timeVertices - timeStart)
              .count();
    std::cout << "vertices parse time: " << verticeParseTime << " ms"
              << std::endl;

    if (!polygonStrings.empty()) {
        polygons->reserve(polygonStrings.size());

#pragma omp parallel if (!_IS_DEBUG)
        {
            auto polygonsPrivate = std::vector<Triangle>();
            polygonsPrivate.reserve(
                polygonStrings.size() / omp_get_max_threads());

#pragma omp for
            for (const auto& string : polygonStrings) {
                for (const auto& polygon : parsePolygon(string)) {
                    polygonsPrivate.push_back(polygon);
                }
            }

#pragma omp critical
            std::copy(
                polygonsPrivate.begin(),
                polygonsPrivate.end(),
                std::back_inserter(*polygons));
        }
    }

    if (!lineStrings.empty()) {
        linesId->reserve(lineStrings.size());

        for (const auto& string : lineStrings) {
            const auto line = parseGlmVec<2U, GLuint>(string);
            linesId->push_back(line);
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
    std::cout << "Strings parse time: " << trianglesParseTime << " ms"
              << std::endl;
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    ObjParseResult result { std::move(vertices),
                            std::move(nVertices),
                            std::move(tVertices),
                            std::move(polygons),
                            std::move(linesId) };

    reset();

    return result;
}

std::vector<EntryIds> ObjParser::parseVertexIds(const std::string_view line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != ObjEntryType::Polygon) {
        throw std::logic_error("Could not parse polygon");
    }

    const auto polygonEntriesStrings = splitPolygonEntries(line);

    std::vector<EntryIds> accumulator {};
    accumulator.resize(polygonEntriesStrings.size());

    std::for_each(
        std::execution::seq,
        polygonEntriesStrings.begin(),
        polygonEntriesStrings.end(),
        [id = 0, &acc = accumulator](auto&& entryString) mutable {
            acc.at(id) = EntryIds::parse(entryString);
            ++id;
        });

    return accumulator;
}

std::vector<std::string_view>
ObjParser::splitPolygonEntries(const std::string_view line)
{
    std::vector<std::string_view> entriesString;
    size_t startPos = 2;

    while (startPos != std::string_view::npos) {
        size_t endPos = line.find(' ', startPos);

        if (endPos == std::string_view::npos) {
            entriesString.push_back(line.substr(startPos));
            break;
        }

        entriesString.push_back(line.substr(startPos, endPos - startPos));
        startPos = endPos + 1;
    }

    return entriesString;
}

std::vector<Triangle> ObjParser::parsePolygon(const std::string_view line)
{
    const auto vertexIds = parseVertexIds(line);

    std::vector<std::pair<glm::vec4, EntryIds>> polygonVertices;

    const auto polygonSize = vertexIds.size();
    for (int i = 0; i != polygonSize; ++i) {
        polygonVertices.emplace_back(vertices->at(i), vertexIds.at(i));
    }

    return EarClipper::triangulate(polygonVertices);
}
