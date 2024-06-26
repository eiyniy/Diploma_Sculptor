#include <ObjParseResult.hpp>

#include <Triangle.hpp>

#include <type_vec2.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <utility>

ObjParseResult::ObjParseResult(
    std::unique_ptr<std::vector<glm::vec4>> _vertices,
    std::unique_ptr<std::vector<glm::vec3>> _nVertices,
    std::unique_ptr<std::vector<glm::vec2>> _tVertices,
    std::unique_ptr<std::vector<Triangle>> _triangles,
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> _linesId)
    : vertices(std::move(_vertices))
    , nVertices(std::move(_nVertices))
    , tVertices(std::move(_tVertices))
    , triangles(std::move(_triangles))
    , linesId(std::move(_linesId))
{
}

std::unique_ptr<std::vector<glm::vec4>> ObjParseResult::getVertices()
{
    return std::move(vertices);
}

std::unique_ptr<std::vector<glm::vec3>> ObjParseResult::getNVertices()
{
    return std::move(nVertices);
}

std::unique_ptr<std::vector<glm::vec2>> ObjParseResult::getTVertices()
{
    return std::move(tVertices);
}

std::unique_ptr<std::vector<Triangle>> ObjParseResult::getTriangles()
{
    if (triangles == nullptr) {
        return nullptr;
    }

    return std::move(triangles);
}

std::unique_ptr<std::vector<glm::vec<2, GLuint>>> ObjParseResult::getLinesId()
{
    if (linesId == nullptr) {
        return nullptr;
    }

    return std::move(linesId);
}