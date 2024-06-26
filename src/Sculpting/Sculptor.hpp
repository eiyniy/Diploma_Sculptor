#pragma once

#include <Brush.hpp>

#include <matrix_float4x4.hpp>
#include <vector_float3.hpp>

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

// TODO: remove static from functions && store Brush
// TODO: Store some glm::vec3 direction in Brush to make different brushes.
// Multiple interpolated to vertex position direction by normals
class Sculptor {
private:
    Brush brush;

public:
    Sculptor(Brush _brush);

    // void createGraph(OldObject* object);

    static void getRayWorld(
        std::pair<float, float> mousePos,
        glm::vec3 cameraPos,
        std::pair<int, int> resolution,
        const glm::mat4& projectionMatrix,
        const glm::mat4& viewMatrix,
        glm::vec3& rayOrig,
        glm::vec3& rayDir);

    static bool intersectRayTriangleGLM(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir,
        const std::array<glm::vec3, 3>& vertices,
        glm::vec3& tuv);

    static int intersectRayTriangleOrig(
        std::array<double, 3> orig,
        std::array<double, 3> dir,
        std::array<double, 3> vert0,
        std::array<double, 3> vert1,
        std::array<double, 3> vert2,
        float& t,
        float& u,
        float& v);

    std::vector<std::array<std::size_t, 3>>
    getSelectedVerticesId(std::array<std::size_t, 3>&& intersectionVerticesId);

    // TODO: accept Brush
    std::vector<std::pair<std::size_t, glm::vec3>> getTransform(
        std::vector<std::size_t>&& verticesId,
        glm::vec3 normal,
        bool isInverted);
};
