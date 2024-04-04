#include <Material.hpp>

#include <Matrix.hpp>

#include <memory>
#include <optional>
#include <string>

class OldTexture;

Material::Material(
    const std::string &_name,
    const std::optional<Vector<4>> &_ambient,
    const std::optional<Vector<4>> &_diffuse,
    const std::optional<Vector<4>> &_specular,
    const std::optional<double> _specularExp,
    std::shared_ptr<const OldTexture> _diffuseMap,
    std::shared_ptr<const OldTexture> _emissiveMap,
    std::shared_ptr<const OldTexture> _normalMap,
    std::shared_ptr<const OldTexture> _mraoMap)
    : name(_name),
      ambient(_ambient),
      diffuse(_diffuse),
      specular(_specular),
      specularExp(_specularExp),
      diffuseMap(_diffuseMap),
      emissiveMap(_emissiveMap),
      normalMap(_normalMap),
      mraoMap(_mraoMap) {}
