#pragma once

#include <BaseTextParser.hpp>
#include <Enums.hpp>
#include <Triangle.hpp>

#include <vec4.hpp>

#include <array>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class OldObject;
class Material;

class ObjParser : public BaseTextParser {
public:
    explicit ObjParser(const std::string& _pathToObj);

    OldObject* parse();

    static std::optional<ObjEntryType> getEntryType(const std::string& line);

private:
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> nVertices;
    std::vector<glm::vec4> tVertices;
    std::vector<Triangle> polygons;
    std::vector<std::pair<std::string, std::optional<std::string>>>
        polygonsStringAndMaterial;

    std::string mtlPath;
    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
        materials;

    std::optional<std::string> currMaterialName;

    void parseEntry(const std::string& line);

    static std::array<std::optional<double>, 4>
    parseAcc(const std::string& line);

    static glm::vec4
    parseVertex(const std::array<std::optional<double>, 4>& acc);

    static glm::vec4
    parseNVertex(const std::array<std::optional<double>, 4>& acc);

    static glm::vec4
    parseTVertex(const std::array<std::optional<double>, 4>& acc);
};
