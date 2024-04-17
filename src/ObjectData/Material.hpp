#pragma once

#include <OldTexture.hpp>

#include <vector_float4.hpp>

#include <memory>
#include <optional>
#include <string>

class Material {
public:
    Material(
        std::string _name,
        const std::optional<glm::vec4>& _ambient,
        const std::optional<glm::vec4>& _diffuse,
        const std::optional<glm::vec4>& _specular,
        const std::optional<double>& _specularExp,
        std::shared_ptr<const OldTexture> _diffuseMap,
        std::shared_ptr<const OldTexture> _emissiveMap,
        std::shared_ptr<const OldTexture> _normalMap,
        std::shared_ptr<const OldTexture> _mraoMap);

    std::shared_ptr<const OldTexture> cGetDiffuseMap() const;
    std::shared_ptr<const OldTexture> cGetEmissiveMap() const;
    std::shared_ptr<const OldTexture> cGetNormalMap() const;
    std::shared_ptr<const OldTexture> cGetMRAOMap() const;

    const std::string& cGetName() const;
    std::optional<glm::vec4> cGetDiffuseCoeff() const;
    std::optional<glm::vec4> cGetAmbientCoeff() const;
    std::optional<glm::vec4> cGetSpecularCoeff() const;
    std::optional<double> cGetSpecularExp() const;

private:
    std::string name;

    // TODO: Remove optional's from class fields
    std::optional<glm::vec4> ambient;
    std::optional<glm::vec4> diffuse;
    std::optional<glm::vec4> specular;
    std::optional<double> specularExp;

    std::shared_ptr<const OldTexture> diffuseMap;
    std::shared_ptr<const OldTexture> emissiveMap;
    std::shared_ptr<const OldTexture> normalMap;
    std::shared_ptr<const OldTexture> mraoMap;
};

inline std::shared_ptr<const OldTexture> Material::cGetDiffuseMap() const
{
    return diffuseMap;
}

inline std::shared_ptr<const OldTexture> Material::cGetEmissiveMap() const
{
    return emissiveMap;
}

inline std::shared_ptr<const OldTexture> Material::cGetNormalMap() const
{
    return normalMap;
}

inline std::shared_ptr<const OldTexture> Material::cGetMRAOMap() const
{
    return mraoMap;
}

inline const std::string& Material::cGetName() const { return name; }

inline std::optional<glm::vec4> Material::cGetDiffuseCoeff() const
{
    return diffuse;
}

inline std::optional<glm::vec4> Material::cGetAmbientCoeff() const
{
    return ambient;
}

inline std::optional<glm::vec4> Material::cGetSpecularCoeff() const
{
    return specular;
}

inline std::optional<double> Material::cGetSpecularExp() const
{
    return specularExp;
}
