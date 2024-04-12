#pragma once

#include <Material.hpp>
#include <Matrix.hpp>
#include <Triangle.hpp>

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
        const std::vector<Vector<4>>& _vertices,
        const std::vector<Vector<4>>& _tVertices,
        const std::vector<Vector<4>>& _nVertices,
        const std::vector<Triangle>& _polygons,
        std::unique_ptr<
            const std::map<std::string, std::shared_ptr<const Material>>>
            _materials);

    void move(const Vector<4>& transition);

    const std::vector<Vector<4>>& cGetVertices() const;

    const std::vector<Vector<4>>& cGetTVertices() const;

    const std::vector<Vector<4>>& cGetNVertices() const;

    const std::vector<Triangle>& cGetPolygons() const;

    std::shared_ptr<const Material> cGetMaterial(const std::string& name) const;
    static std::shared_ptr<const Material> getDefaultMaterial();

    std::vector<Vector<4>>& getVertices();

    std::vector<Triangle>& getPolygons();

    void convertToDrawable(const Camera& camera);

    const std::vector<Vector<4>>& cGetDrawable() const;

    const Vector<4>& getCenter();

    const Vector<4>& getMaxXZ();

    const Vector<4>& getMinXZ();

private:
    void calcGeometricParams();

    std::vector<Vector<4>> vertices;
    std::vector<Vector<4>> nVertices;
    std::vector<Vector<4>> tVertices;
    std::vector<Triangle> polygons;

    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
        materials;
    static std::shared_ptr<const Material> defaultMaterial;

    std::optional<Vector<4>> center;
    std::optional<Vector<4>> maxXZ, minXZ;

    std::vector<Vector<4>> drawable;
};

inline const std::vector<Vector<4>>& OldObject::cGetDrawable() const
{
    return drawable;
}

inline const std::vector<Vector<4>>& OldObject::cGetVertices() const
{
    return vertices;
}

inline const std::vector<Vector<4>>& OldObject::cGetTVertices() const
{
    return tVertices;
}

inline const std::vector<Vector<4>>& OldObject::cGetNVertices() const
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

inline std::vector<Vector<4>>& OldObject::getVertices() { return vertices; }

inline std::vector<Triangle>& OldObject::getPolygons() { return polygons; }
