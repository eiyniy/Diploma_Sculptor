#include <OldObject.hpp>

#include <Globals.hpp>
#include <Material.hpp>
#include <Triangle.hpp>

#include <mat4x4.hpp>
#include <vec4.hpp>

#include <memory>
#include <vector>

glm::vec4 convertVertex(
    const glm::vec4& vertex,
    const glm::mat4x4& toProjectionConvert,
    const glm::mat4x4& viewportConvert);

std::shared_ptr<const Material> OldObject::defaultMaterial
    = std::make_shared<const Material>(
        "__DEFAULT_MATERIAL",
        glm::vec4 { 0, 0, 0, 0 },
        glm::vec4 { 1, 1, 1, 0 },
        glm::vec4 { 0, 0, 0, 0 },
        1,
        nullptr,
        nullptr,
        nullptr,
        nullptr);

OldObject::OldObject(
    const std::vector<glm::vec4>& _vertices,
    const std::vector<glm::vec4>& _tVertices,
    const std::vector<glm::vec4>& _nVertices,
    const std::vector<Triangle>& _polygons,
    std::unique_ptr<
        const std::map<std::string, std::shared_ptr<const Material>>>
        _materials)
    : vertices(_vertices)
    , tVertices(_tVertices)
    , nVertices(_nVertices)
    , polygons(_polygons)
    , materials(std::move(_materials))
{
    drawable = std::vector<glm::vec4>(vertices.size());
}

void OldObject::move(const glm::vec4& transition)
{
    /*
    const auto moveConvert = Matrix<4, 4>::getMoveConvert(transition);

    // vertices[0].log();

#pragma omp parallel for if (!_IS_DEBUG)
    for (auto& vertice : vertices) {
        vertice = moveConvert * vertice;
    }

    // vertices[0].log();
    // std::cout << std::endl;
    */
}

void OldObject::convertToDrawable(const Camera& camera)
{
    /*
    const auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.cGetFOV(),
camera.cGetAspect(), 2000, 0.1) * Matrix<4,
4>::getViewConvert(camera.cGetPosition(), camera.cGetTarget(), camera.cGetUp());

    const auto &resolution = camera.cGetResolution();
    const auto viewportConvert = Matrix<4, 4>::getViewportConvert(
        resolution.first, resolution.second, 0, 0);

#pragma omp parallel for if (!_IS_DEBUG)
    for (int j = 0; j < vertices.size(); ++j)
        drawable[j] = convertVertex(vertices[j], convertMatrix,
viewportConvert);
    */
}

void OldObject::calcGeometricParams()
{
    double cx = 0;
    double cy = 0;
    double cz = 0;

    auto first = vertices.cbegin();
    double xMax = first->x;
    double xMin = first->x;
    double zMax = first->z;
    double zMin = first->z;

    for (auto&& vertex : vertices) {
        const auto x = vertex.x;
        const auto y = vertex.y;
        const auto z = vertex.z;

        if (x > xMax) {
            xMax = x;
        } else if (x < xMin) {
            xMin = x;
        }
        if (z > zMax) {
            zMax = z;
        } else if (z < zMin) {
            zMin = z;
        }

        cx += x;
        cy += y;
        cz += z;
    }

    const auto vCount = (double)vertices.size();

    center = glm::vec4(cx / vCount, cy / vCount, cz / vCount, 1.F);
    maxXZ = glm::vec4(xMax, 0, zMax, 1.F);
    minXZ = glm::vec4(xMin, 0, zMin, 1.F);
}

const glm::vec4& OldObject::getCenter()
{
    // if (!center.has_value())
    calcGeometricParams();

    return *center;
}

const glm::vec4& OldObject::getMaxXZ()
{
    // if (!maxXZ.has_value())
    calcGeometricParams();

    return *maxXZ;
}

const glm::vec4& OldObject::getMinXZ()
{
    // if (!minXZ.has_value())
    calcGeometricParams();

    return *minXZ;
}

glm::vec4 convertVertex(
    const glm::vec4& vertex,
    const glm::mat4x4& toProjectionConvert,
    const glm::mat4x4& viewportConvert)
{
    bool isWNegative = false;

    auto vertexCopy = toProjectionConvert * vertex;

    if (vertexCopy.w <= 0) {
        isWNegative = true;
    }

    const auto w = vertexCopy.w;
    const auto z = vertexCopy.z;
    vertexCopy /= w;

    vertexCopy = viewportConvert * vertexCopy;

    vertexCopy.z = z;
    vertexCopy.w = w;

    return vertexCopy;
}
