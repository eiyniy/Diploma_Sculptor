#pragma once

#include <Graph.hpp>

#include <matrix_float4x4.hpp>
#include <optional>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <cstddef>

class Sculptor {
private:
    int radius;

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

public:
    Sculptor(int _radius);

    [[nodiscard]] int cGetRadius() const;

    // void createGraph(OldObject* object);

    // void pull(
    //     std::vector<glm::vec4>& vertices,
    //     std::pair<int, int> mousePos,
    //     const glm::vec4& direction);

    // static void
    // pull(OldObject* object, int triangleId, std::pair<int, int> mousePos);

    static void getRayWorld(
        std::pair<float, float> mousePos,
        glm::vec3 cameraPos,
        std::pair<int, int> resolution,
        const glm::mat4& projectionMatrix,
        const glm::mat4& viewMatrix,
        glm::vec3& rayOrig,
        glm::vec3& rayDir);

    static std::optional<size_t> getSelectedTriangleId(
        const std::vector<GLfloat>& trVertices,
        const std::vector<GLuint>& indices,
        glm::vec3 rayOrig,
        glm::vec3 rayDir);
};
