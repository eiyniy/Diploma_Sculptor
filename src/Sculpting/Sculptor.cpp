#include "matrix_float4x4.hpp"
#include "vector_float3.hpp"
#include <Sculptor.hpp>

#include <Graph.hpp>
#include <Triangle.hpp>

#include <geometric.hpp>
#include <matrix.hpp>
#include <optional>
#include <type_vec4.hpp>
#include <vector_float4.hpp>

#include <array>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

Sculptor::Sculptor(const int _radius)
    : radius(_radius)
{
}

// void Sculptor::createGraph(OldObject* object)
// {
//     graph = Graph(object->cGetDrawable(), object->cGetPolygons());
// }

// void Sculptor::pull(
//     std::vector<glm::vec4>& vertices,
//     const std::pair<int, int> mousePos,
//     const glm::vec4& direction)
// {
//     const auto affectedVerticesIds
//         = graph.getAffectedVerticesIds(mousePos, radius);

//     // std::cout << "Sculptor log: " << std::endl;
//     // std::cout << "Affected Vertices count: " <<
//     affectedVerticesIds->size()
//         //   << std::endl;

//         for (auto&& vertexId : *affectedVerticesIds)
//     {
//         vertices[vertexId] += direction;
//     }
// }

// void Sculptor::pull(
//     OldObject* object, const int triangleId, const std::pair<int, int>
//     mousePos)
// {
//     // std::cout << "      " << triangleId << std::endl;

//     auto& vertices = object->getVertices();
//     auto polygon = object->getPolygons().at(triangleId);

//     const auto normal = polygon.getFlatNormal(vertices);

//     const auto vId0 = polygon.cGetVertexIds(0).cGetVertexId() - 1;
//     const auto vId1 = polygon.cGetVertexIds(1).cGetVertexId() - 1;
//     const auto vId2 = polygon.cGetVertexIds(2).cGetVertexId() - 1;

//     // std::cout << vertices[vId0].cGetX() << std::endl;

//     vertices[vId0] += normal;
//     vertices[vId1] += normal;
//     vertices[vId2] += normal;

//     // std::cout << vertices[vId0].cGetX() << std::endl;
// }

int Sculptor::cGetRadius() const { return radius; }

std::optional<size_t> Sculptor::getSelectedTriangleId(
    const std::vector<GLfloat>& trVertices,
    const std::vector<GLuint>& indices,
    const glm::vec3 rayOrig,
    const glm::vec3 rayDir)
{
    for (size_t triangleId = 0; triangleId < indices.size(); triangleId += 3) {
        std::array<glm::vec3, 3> triangleVertices {};

        for (int vertexId = 0; vertexId < 3; ++vertexId) {
            const auto trIndex = triangleId * 3;
            const glm::vec3 vertex { trVertices.at(trIndex),
                                     trVertices.at(trIndex + 1),
                                     trVertices.at(trIndex + 2) };

            triangleVertices.at(vertexId) = vertex;
        }

        glm::vec3 tuv;
        if (intersectRayTriangleGLM(rayOrig, rayDir, triangleVertices, tuv)) {
            return triangleId;
        }
    }

    return std::nullopt;
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

    if (det > 0.000001F) {
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

    } else if (det < -0.000001F) {
        /* calculate U parameter and test bounds */
        tuv.y = glm::dot(tVec, pVec);
        if (tuv.y > 0.F || tuv.y < det) {
            return false;
        }

        /* prepare to test V parameter */
        qVec = glm::cross(tVec, edge1);

        /* calculate V parameter and test bounds */
        tuv.z = glm::dot(rayDir, qVec);
        if (tuv.z > 0.0 || tuv.y + tuv.z < det) {
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

void Sculptor::getRayWorld(
    std::pair<float, float> mousePos,
    std::pair<int, int> resolution,
    const glm::mat4& projectionMatrix,
    const glm::mat4& viewMatrix,
    glm::vec3& rayOrig,
    glm::vec3& rayDir)
{
    /*
    const glm::vec3 ray_nds { x, y, z };
    const glm::vec4 ray_clip { ray_nds.x, ray_nds.y, -1.0, 1.0 };
    glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.x, -1.0, 0.0);
    glm::vec3 ray_wor { glm::inverse(view_matrix) * ray_eye };
    ray_wor = glm::normalize(ray_wor);
    */

    const float mouseXNdc
        = (2.F * mousePos.first) / static_cast<float>(resolution.first) - 1.F;
    const float mouseYNdc
        = 1.F - (2.F * mousePos.second) / static_cast<float>(resolution.second);

    const glm::vec4 rayStartNdc { mouseXNdc, mouseYNdc, -1.F, 1.F };
    const glm::vec4 rayEndNdc { mouseXNdc, mouseYNdc, 0.F, 1.F };

    const glm::mat4 viewportToWorld
        = glm::inverse(projectionMatrix * viewMatrix);
    glm::vec4 rayStartWorld = viewportToWorld * rayStartNdc;
    rayStartWorld /= rayStartWorld.w;
    glm::vec4 rayEndWorld = viewportToWorld * rayEndNdc;
    rayEndWorld /= rayEndWorld.w;

    rayOrig = rayStartWorld;
    rayDir = glm::normalize(rayEndWorld - rayStartWorld);
}
