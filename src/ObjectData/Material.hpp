#pragma once

#include <vector_float4.hpp>

#include <optional>
#include <string>

class Material {
public:
    Material(
        std::string _name,
        const std::optional<glm::vec4>& _ambient,
        const std::optional<glm::vec4>& _diffuse,
        const std::optional<glm::vec4>& _specular,
        const std::optional<double>& _specularExp);

    [[nodiscard]] const std::string& cGetName() const;
    [[nodiscard]] std::optional<glm::vec4> cGetDiffuseCoeff() const;
    [[nodiscard]] std::optional<glm::vec4> cGetAmbientCoeff() const;
    [[nodiscard]] std::optional<glm::vec4> cGetSpecularCoeff() const;
    [[nodiscard]] std::optional<double> cGetSpecularExp() const;

private:
    std::string name;

    // TODO: Remove optional's from class fields
    std::optional<glm::vec4> ambient;
    std::optional<glm::vec4> diffuse;
    std::optional<glm::vec4> specular;
    std::optional<double> specularExp;
};
