#include <Material.hpp>

#include <vector_float4.hpp>

#include <optional>
#include <string>
#include <utility>

Material::Material(
    std::string _name,
    const std::optional<glm::vec4>& _ambient,
    const std::optional<glm::vec4>& _diffuse,
    const std::optional<glm::vec4>& _specular,
    const std::optional<double>& _specularExp)
    : name(std::move(_name))
    , ambient(_ambient)
    , diffuse(_diffuse)
    , specular(_specular)
    , specularExp(_specularExp)
{
}

const std::string& Material::cGetName() const { return name; }

std::optional<glm::vec4> Material::cGetDiffuseCoeff() const { return diffuse; }

std::optional<glm::vec4> Material::cGetAmbientCoeff() const { return ambient; }

std::optional<glm::vec4> Material::cGetSpecularCoeff() const
{
    return specular;
}

std::optional<double> Material::cGetSpecularExp() const { return specularExp; }
