#include <Sculptor.hpp>

#include <Brush.hpp>
#include <Math.hpp>

#include <geometric.hpp>
#include <matrix.hpp>
#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <array>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

Sculptor::Sculptor(const Brush _brush)
    : brush(_brush)
{
}

// void Sculptor::createGraph(OldObject* object)
// {
//     graph = Graph(object->cGetDrawable(), object->cGetPolygons());
// }

void Sculptor::getRayWorld(
    std::pair<float, float> mousePos,
    glm::vec3 cameraPos,
    std::pair<int, int> resolution,
    const glm::mat4& projectionMatrix,
    const glm::mat4& viewMatrix,
    glm::vec3& rayOrig,
    glm::vec3& rayDir)
{
    const float mouseXNdc
        = (2.F * mousePos.first) / static_cast<float>(resolution.first) - 1.F;
    const float mouseYNdc
        = 1.F - (2.F * mousePos.second) / static_cast<float>(resolution.second);

    glm::vec3 rayStartWorld = cameraPos;
    const glm::vec4 rayEndNdc { mouseXNdc, mouseYNdc, 0.F, 1.F };

    const glm::mat4 viewportToWorld
        = glm::inverse(projectionMatrix * viewMatrix);
    glm::vec4 rayEndWorld = viewportToWorld * rayEndNdc;
    rayEndWorld /= rayEndWorld.w;

    rayOrig = rayStartWorld;
    rayDir = glm::normalize(glm::vec3(rayEndWorld) - rayStartWorld);
}

std::vector<std::array<std::size_t, 3>> Sculptor::getSelectedVerticesId(
    std::array<std::size_t, 3>&& intersectionVerticesId)
{
    // TODO: Find vertices in radius
}

bool Sculptor::intersectRayTriangleGLM(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const std::array<glm::vec3, 3>& vertices,
    glm::vec3& tuv)
{
    glm::vec3 tVec;
    glm::vec3 pVec;
    glm::vec3 qVec;

    float det = NAN;
    float invDet = NAN;

    /* find vectors for two edges sharing vert0 */
    glm::vec3 edge1 = vertices[1] - vertices[0];
    glm::vec3 edge2 = vertices[2] - vertices[0];

    /* begin calculating determinant - also used to calculate U parameter */
    pVec = glm::cross(rayDir, edge2);

    /* if determinant is near zero, ray lies in plane of triangle */
    det = glm::dot(edge1, pVec);

    /* calculate distance from vert0 to ray origin */
    tVec = rayOrigin - vertices[0];
    invDet = 1.F / det;

    if (det > Math::fastEpsilon) {
        /* calculate U parameter and test bounds */
        tuv.y = glm::dot(tVec, pVec);
        if (tuv.y < 0.F || tuv.y > det) {
            return false;
        }

        /* prepare to test V parameter */
        qVec = glm::cross(tVec, edge1);

        /* calculate V parameter and test bounds */
        tuv.z = glm::dot(rayDir, qVec);
        if (tuv.z < 0.F || tuv.y + tuv.z > det) {
            return false;
        }
    } else if (det < -Math::fastEpsilon) {
        /* calculate U parameter and test bounds */
        tuv.y = glm::dot(tVec, pVec);
        if (tuv.y > 0.F || tuv.y < det) {
            return false;
        }

        /* prepare to test V parameter */
        qVec = glm::cross(tVec, edge1);

        /* calculate V parameter and test bounds */
        tuv.z = glm::dot(rayDir, qVec);
        if (tuv.z > 0.F || tuv.y + tuv.z < det) {
            return false;
        }
    } else {
        return false; /* ray is parallell to the plane of the triangle */
    }

    /* calculate t, ray intersects triangle */
    tuv.x = glm::dot(edge2, qVec) * invDet;
    tuv.y *= invDet;
    tuv.z *= invDet;

    return true;
}

std::vector<std::pair<std::size_t, glm::vec3>> Sculptor::getTransform(
    std::vector<std::size_t>&& verticesId,
    const glm::vec3 normal,
    const bool isInverted)
{
    std::vector<std::pair<std::size_t, glm::vec3>> transform;
    transform.resize(verticesId.size());

    for (std::size_t i = 0; i < verticesId.size(); ++i) {
        const auto signedStrengthF = isInverted
            ? -static_cast<float>(brush.getStrength())
            : static_cast<float>(brush.getStrength());

        const auto transformValue = normal * signedStrengthF;
        transform.at(i) = std::make_pair(verticesId.at(i), transformValue);
    }

    return transform;
}
