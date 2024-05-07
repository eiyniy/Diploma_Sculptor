#pragma once

#include <BaseTextParser.hpp>
#include <Enums.hpp>
#include <Material.hpp>

#include <vector_float4.hpp>

#include <map>
#include <memory>
#include <optional>
#include <string>

class MtlParser : public BaseTextParser {
public:
    explicit MtlParser();

    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
    parse(const std::string& pathToMtl);

private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<const Material>>>
        materials;

    std::string name;
    std::optional<glm::vec4> ambient;
    std::optional<glm::vec4> diffuse;
    std::optional<glm::vec4> specular;
    std::optional<double> specularExp;

    void parseEntry(const std::string& line);

    static std::optional<MtlEntryType> getEntryType(const std::string& line);

    void resetMaterial();

    static glm::vec4 parseCoeff(const std::string& line);
};
