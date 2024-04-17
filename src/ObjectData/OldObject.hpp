#pragma once

#include <Material.hpp>
#include <Triangle.hpp>

#include <vector_float4.hpp>

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

class Camera;

class OldObject {
public:
    OldObject(
        const std::vector<glm::vec4>& _vertices,
        const std::vector<glm::vec4>& _tVertices,
        const std::vector<glm::vec4>& _nVertices,
        const std::vector<Triangle>& _polygons,
        std::unique_ptr<
            const std::map<std::string, std::shared_ptr<const Material>>>
            _materials);

    void move(const glm::vec4& transition);

    const std::vector<glm::vec4>& cGetVertices() const;

    const std::vector<glm::vec4>& cGetTVertices() const;

    const std::vector<glm::vec4>& cGetNVertices() const;

    const std::vector<Triangle>& cGetPolygons() const;

    std::shared_ptr<const Material> cGetMaterial(const std::string& name) const;
    static std::shared_ptr<const Material> getDefaultMaterial();

    std::vector<glm::vec4>& getVertices();

    std::vector<Triangle>& getPolygons();

    void convertToDrawable(const Camera& camera);

    const std::vector<glm::vec4>& cGetDrawable() const;

    const glm::vec4& getCenter();

    const glm::vec4& getMaxXZ();

    const glm::vec4& getMinXZ();

private:
    void calcGeometricParams();

    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> nVertices;
    std::vector<glm::vec4> tVertices;
    std::vector<Triangle> polygons;

    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
        materials;
    static std::shared_ptr<const Material> defaultMaterial;

    std::optional<glm::vec4> center;
    std::optional<glm::vec4> maxXZ, minXZ;

    std::vector<glm::vec4> drawable;
};

inline const std::vector<glm::vec4>& OldObject::cGetDrawable() const
{
    return drawable;
}

inline const std::vector<glm::vec4>& OldObject::cGetVertices() const
{
    return vertices;
}

inline const std::vector<glm::vec4>& OldObject::cGetTVertices() const
{
    return tVertices;
}

inline const std::vector<glm::vec4>& OldObject::cGetNVertices() const
{
    return nVertices;
}

inline const std::vector<Triangle>& OldObject::cGetPolygons() const
{
    return polygons;
}

inline std::shared_ptr<const Material>
OldObject::cGetMaterial(const std::string& name) const
{
    if (!materials->contains(name)) {
        throw std::runtime_error("Can't get material");
    }

    return materials->at(name);
}

inline std::shared_ptr<const Material> OldObject::getDefaultMaterial()
{
    return defaultMaterial;
}

inline std::vector<glm::vec4>& OldObject::getVertices() { return vertices; }

inline std::vector<Triangle>& OldObject::getPolygons() { return polygons; }
