#pragma once

#include <BaseTextParser.hpp>
#include <Camera.hpp>
#include <Concepts.hpp>
#include <EntryIds.hpp>
#include <Enums.hpp>
#include <ObjParseResult.hpp>
#include <Triangle.hpp>

#include <qualifier.hpp>
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
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> linesId;

    std::vector<std::string> polygonStrings;
    std::vector<std::string> lineStrings;

    // TODO: Add reset() = 0 to BaseTextParser
    void reset();

    void parseEntry(const std::string& line);

    template <int L, NumericType V>
        requires GlmVec<L, V>
    glm::vec<L, V> parseGlmVec(const std::string& line);

    template <int L, NumericType T>
        requires GlmVecLength<L>
    static void fillVec(glm::vec<L, T>& vec, char index, T value);

    static std::vector<EntryIds> parseVertexIds(const std::string& line);
    std::vector<Triangle> parsePolygon(const std::string& line);

public:
    ObjParser();

    ObjParser(const ObjParser&) = delete;
    ObjParser(ObjParser&&) noexcept = default;
    ObjParser& operator=(const ObjParser&) = delete;
    ObjParser& operator=(ObjParser&&) noexcept = default;

    ~ObjParser() = default;

    ObjParseResult parse(const std::string& pathToObj);

    static std::optional<ObjEntryType> getEntryType(const std::string& line);
};

template <int L, NumericType V>
    requires GlmVec<L, V>
glm::vec<L, V> ObjParser::parseGlmVec(const std::string& line)
{
    glm::vec<L, V> vec { 1 };

    std::size_t pos = 0;
    for (char index = 0; index < 4; ++index) {
        std::size_t start = line.find_first_of("0123456789.-", pos);
        if (start == std::string::npos) {
            break;
        }

        std::size_t end = 0;

        V extractedValue {};

        if constexpr (std::is_integral_v<V>) {
            extractedValue = std::stoi(line.substr(start), &end);
        } else if constexpr (std::is_floating_point_v<V>) {
            extractedValue = std::stof(line.substr(start), &end);
        }

        fillVec(vec, index, extractedValue);

        pos = start + end;
    }

    return vec;
}

template <int L, NumericType T>
    requires GlmVecLength<L>
void ObjParser::fillVec(glm::vec<L, T>& vec, char index, T value)
{
    if (index == 0) {
        vec.x = value;
    } else if (index == 1) {
        vec.y = value;
    } else if (index == 2) {
        if constexpr (L > 2) {
            vec.z = value;
        }
    } else if (index == 3) {
        if constexpr (L > 3) {
            vec.w = value;
        }
    }
}
