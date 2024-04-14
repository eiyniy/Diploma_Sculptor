#pragma once

#include <BaseTextParser.hpp>
#include <Enums.hpp>
#include <Material.hpp>
#include <OldTexture.hpp>

#include <vec4.hpp>

#include <map>
#include <memory>
#include <optional>
#include <string>

class MtlParser : public BaseTextParser {
public:
    explicit MtlParser(const std::string& _pathToMtl);

    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
    parse();

private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<const Material>>>
        materials;

    std::string name;
    std::optional<glm::vec4> ambient;
    std::optional<glm::vec4> diffuse;
    std::optional<glm::vec4> specular;
    std::optional<double> specularExp;
    std::unique_ptr<const OldTexture> diffuseMap;
    std::unique_ptr<const OldTexture> emissiveMap;
    std::unique_ptr<const OldTexture> normalMap;
    std::unique_ptr<const OldTexture> mraoMap;

    void parseEntry(const std::string& line);

    static std::optional<MtlEntryType> getEntryType(const std::string& line);

    void resetMaterial();

    static glm::vec4 parseCoeff(const std::string& line);
    static std::unique_ptr<const OldTexture> parseTexture(
        const std::string& line,
        const std::string& pathToFile,
        std::optional<MtlEntryType> optType = std::nullopt);
};
