#pragma once

#include <Triangle.hpp>

#include <qualifier.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <memory>
#include <vector>

class ObjParseResult {

    friend class ObjParseResultBuilder;

private:
    std::unique_ptr<std::vector<glm::vec4>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> nVertices;
    std::unique_ptr<std::vector<glm::vec2>> tVertices;
    std::unique_ptr<std::vector<Triangle>> triangles;
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> linesId;

public:
    ObjParseResult(
        std::unique_ptr<std::vector<glm::vec4>> _vertices,
        std::unique_ptr<std::vector<glm::vec3>> _nVertices,
        std::unique_ptr<std::vector<glm::vec2>> _tVertices,
        std::unique_ptr<std::vector<Triangle>> _triangles = nullptr,
        std::unique_ptr<std::vector<glm::vec<2, GLuint>>> _linesId = nullptr);

    std::unique_ptr<std::vector<glm::vec4>> getVertices();
    std::unique_ptr<std::vector<glm::vec3>> getNVertices();
    std::unique_ptr<std::vector<glm::vec2>> getTVertices();
    std::unique_ptr<std::vector<Triangle>> getTriangles();
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> getLinesId();
};