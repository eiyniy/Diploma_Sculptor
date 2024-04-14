#include <Material.hpp>

#include <vec4.hpp>

#include <memory>
#include <optional>
#include <string>
#include <utility>

class OldTexture;

Material::Material(
    std::string _name,
    const std::optional<glm::vec4>& _ambient,
    const std::optional<glm::vec4>& _diffuse,
    const std::optional<glm::vec4>& _specular,
    const std::optional<double>& _specularExp,
    std::shared_ptr<const OldTexture> _diffuseMap,
    std::shared_ptr<const OldTexture> _emissiveMap,
    std::shared_ptr<const OldTexture> _normalMap,
    std::shared_ptr<const OldTexture> _mraoMap)
    : name(std::move(_name))
    , ambient(_ambient)
    , diffuse(_diffuse)
    , specular(_specular)
    , specularExp(_specularExp)
    , diffuseMap(std::move(_diffuseMap))
    , emissiveMap(std::move(_emissiveMap))
    , normalMap(std::move(_normalMap))
    , mraoMap(std::move(_mraoMap))
{
}
