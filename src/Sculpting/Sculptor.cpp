#include <Sculptor.hpp>

#include <Globals.hpp>
#include <Graph.hpp>
#include <Math.hpp>
#include <Triangle.hpp>

#include <geometric.hpp>
#include <matrix.hpp>
#include <matrix_float4x4.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
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

int Sculptor::cGetRadius() const { return radius; }

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

// TODO: Mb move to Object OR make Object::getTriangle(Id)
std::optional<std::array<std::size_t, 3>>
Sculptor::getSelectedTriangleVerticesIds(
    const std::vector<GLfloat>& trVertices,
    const std::vector<GLuint>& indices,
    const glm::vec3 rayOrig,
    const glm::vec3 rayDir)
{
    std::vector<std::pair<std::array<std::size_t, 3>, float>> idsWithDistance;

#pragma omp parallel for if (!_IS_DEBUG)
    for (std::size_t triangleId = 0; triangleId < indices.size();
         triangleId += 3) {
        std::array<glm::vec3, 3> triangleVertices {};

        for (int vertexId = 0; vertexId < 3; ++vertexId) {
            const auto trIndex = indices.at(triangleId + vertexId) * 4;
            triangleVertices.at(vertexId) = { trVertices.at(trIndex),
                                              trVertices.at(trIndex + 1),
                                              trVertices.at(trIndex + 2) };
        }

        glm::vec3 tuv;
        if (intersectRayTriangleGLM(rayOrig, rayDir, triangleVertices, tuv)
            && tuv.x > 0) {
#pragma omp critical(new_result)
            {
                std::array<std::size_t, 3> Ids { indices.at(triangleId),
                                                 indices.at(triangleId + 1),
                                                 indices.at(triangleId + 2) };
                idsWithDistance.emplace_back(Ids, tuv.x);
            }
        }
    }

    if (idsWithDistance.empty()) {
        return std::nullopt;
    }

    auto result = idsWithDistance.front();
    for (auto&& elem : idsWithDistance) {
        if (elem.second < result.second) {
            result = elem;
        }
    }

    return result.first;
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

std::vector<std::pair<std::size_t, glm::vec3>>
Sculptor::getTransform(const std::vector<std::size_t>& verticesId)
{
    std::vector<std::pair<std::size_t, glm::vec3>> transform;
    transform.resize(verticesId.size());

    for (std::size_t i = 0; i < verticesId.size(); ++i) {
        transform.at(i)
            = std::make_pair(verticesId.at(i), glm::vec3 { 1, 0, 0 });
    }

    return transform;
}
