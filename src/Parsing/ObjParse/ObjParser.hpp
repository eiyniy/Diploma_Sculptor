#pragma once

#include <BaseTextParser.hpp>
#include <Concepts.hpp>
#include <EntryIds.hpp>
#include <Enums.hpp>
#include <ObjParseResult.hpp>
#include <Triangle.hpp>

#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// TODO: Use string_view
class ObjParser : public BaseTextParser {
private:
    std::unique_ptr<std::vector<glm::vec4>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> nVertices;
    std::unique_ptr<std::vector<glm::vec2>> tVertices;
    std::unique_ptr<std::vector<Triangle>> polygons;

    std::vector<std::string> polygonStrings;

    void parseEntry(const std::string& line);

    template <class T>
        requires IsGlmVec<T>
    T parseGlmVec(
        const std::string& line, void (*fillFunc)(T&, char index, float value));

    static void fillVec3(glm::vec3& vec, char index, float value);
    static void fillVec2(glm::vec2& vec, char index, float value);
    static void fillVec4(glm::vec4& vec, char index, float value);

    static std::vector<EntryIds> parseVertexIds(const std::string& line);
    std::vector<Triangle> parsePolygon(const std::string& line);

public:
    explicit ObjParser(const std::string& _pathToObj);

    ObjParseResult parse();

    static std::optional<ObjEntryType> getEntryType(const std::string& line);
};

template <class T>
    requires IsGlmVec<T>
T ObjParser::parseGlmVec(
    const std::string& line, void (*fillFunc)(T&, char index, float value))
{
    T vec { 1 };

    std::size_t pos = 0;
    for (char index = 0; index < 4; ++index) {
        std::size_t start = line.find_first_of("0123456789.-", pos);
        if (start == std::string::npos) {
            break;
        }

        std::size_t end = 0;
        float extractedFloat = std::stof(line.substr(start), &end);

        fillFunc(vec, index, extractedFloat);

        pos = start + end;
    }

    return vec;
}
