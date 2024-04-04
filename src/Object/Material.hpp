#pragma once

#include <OldTexture.hpp>
#include <Matrix.hpp>

#include <memory>
#include <string>
#include <optional>

class Material
{
public:
    Material(
        const std::string &_name,
        const std::optional<Vector<4>> &_ambient,
        const std::optional<Vector<4>> &_diffuse,
        const std::optional<Vector<4>> &_specular,
        const std::optional<double> _specularExp,
        std::shared_ptr<const OldTexture> _diffuseMap,
        std::shared_ptr<const OldTexture> _emissiveMap,
        std::shared_ptr<const OldTexture> _normalMap,
        std::shared_ptr<const OldTexture> _mraoMap);

    const std::shared_ptr<const OldTexture> cGetDiffuseMap() const;
    const std::shared_ptr<const OldTexture> cGetEmissiveMap() const;
    const std::shared_ptr<const OldTexture> cGetNormalMap() const;
    const std::shared_ptr<const OldTexture> cGetMRAOMap() const;

    const std::string &cGetName() const;
    const std::optional<Vector<4>> cGetDiffuseCoeff() const;
    const std::optional<Vector<4>> cGetAmbientCoeff() const;
    const std::optional<Vector<4>> cGetSpecularCoeff() const;
    const std::optional<double> cGetSpecularExp() const;

private:
    std::string name;
    std::optional<Vector<4>> ambient;
    std::optional<Vector<4>> diffuse;
    std::optional<Vector<4>> specular;
    std::optional<double> specularExp;
    std::shared_ptr<const OldTexture> diffuseMap;
    std::shared_ptr<const OldTexture> emissiveMap;
    std::shared_ptr<const OldTexture> normalMap;
    std::shared_ptr<const OldTexture> mraoMap;
};

inline const std::shared_ptr<const OldTexture> Material::cGetDiffuseMap() const
{
    return diffuseMap;
}

inline const std::shared_ptr<const OldTexture> Material::cGetEmissiveMap() const
{
    return emissiveMap;
}

inline const std::shared_ptr<const OldTexture> Material::cGetNormalMap() const
{
    return normalMap;
}

inline const std::shared_ptr<const OldTexture> Material::cGetMRAOMap() const
{
    return mraoMap;
}

inline const std::string &Material::cGetName() const
{
    return name;
}

inline const std::optional<Vector<4>> Material::cGetDiffuseCoeff() const
{
    return diffuse;
}

inline const std::optional<Vector<4>> Material::cGetAmbientCoeff() const
{
    return ambient;
}

inline const std::optional<Vector<4>> Material::cGetSpecularCoeff() const
{
    return specular;
}

inline const std::optional<double> Material::cGetSpecularExp() const
{
    return specularExp;
}
