#include <Object.hpp>

#include <Globals.hpp>
#include <Sculptor.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <geometric.hpp>
#include <qualifier.hpp>
#include <type_vec3.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

Object::Object(ConstructorPasskey<Object>&& passkey)
    : VAO(0)
    , drawMode(0)
    , verticesVBO(0)
    , tVerticesVBO(0)
    , nVerticesVBO(0)
    , EBO(0)
    , _isShaderEnabled(false)
{
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &verticesVBO);
    glDeleteBuffers(1, &tVerticesVBO);
    glDeleteBuffers(1, &nVerticesVBO);
    glDeleteBuffers(1, &EBO);
}

void Object::throwIfShaderNotEnabled(
    const std::string& message = std::string()) const
{
    if (!isShaderEnabled()) {
        throw std::logic_error("No one shader is enabled. " + message);
    }
}

void Object::bindTextures()
{
    throwIfShaderNotEnabled("bindTextures");

    if (textures.empty()) {
        throw std::logic_error("Can't bind Textures. Object doesn't has them");
    }

    for (auto&& texture : textures) {
        texture.second->bind();

        loadUniform(
            texture.first, texture.second->getTextureBlock() - GL_TEXTURE0);
    }
}

void Object::bindVerticesVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trVertices.size() * sizeof(GLfloat)),
        trVertices.data(),
        GL_DYNAMIC_DRAW);
}

void Object::bindTVerticesVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, tVerticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trTVertices.size() * sizeof(GLfloat)),
        trTVertices.data(),
        GL_STATIC_DRAW);
}

void Object::bindNVerticesVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, nVerticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trNVertices.size() * sizeof(GLfloat)),
        trNVertices.data(),
        GL_STATIC_DRAW);
}

void Object::bindEBO()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)),
        indices.data(),
        GL_STATIC_DRAW);
}

// NOLINTNEXTLINE
void Object::unbindVBO() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Object::enableShader()
{
    if (isShaderEnabled()) {
        throw std::logic_error("Can't enable shader program. There is another "
                               "one enabled already");
    }

    shaderProgram->enable();

    _isShaderEnabled = true;
}

void Object::disableShader()
{
    if (!isShaderEnabled()) {
        throw std::logic_error(
            "Can't disable shader program. No one of them is enabled");
    }

    shaderProgram->disable();
    _isShaderEnabled = false;
}

void Object::draw() const
{
    throwIfShaderNotEnabled("draw");

    glBindVertexArray(VAO);

    // std::cout << "Draw indices count: " << indices.size() << std::endl;

    glDrawElements(
        drawMode,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        nullptr);

    glBindVertexArray(0);
}

void Object::loadUniforms() { shaderProgram->loadUniforms(); }

void Object::performTransform(
    const std::vector<std::pair<std::size_t, glm::vec3>>& transform)
{
    for (auto&& vertexTransform : transform) {
        const auto extendedTransformPositions
            = connectedIndicesIds.at(vertexTransform.first);

        for (auto&& transformPos : extendedTransformPositions) {
            trVertices.at(transformPos * glm::vec4::length())
                += vertexTransform.second.x;
            trVertices.at(transformPos * glm::vec4::length() + 1)
                += vertexTransform.second.y;
            trVertices.at(transformPos * glm::vec4::length() + 2)
                += vertexTransform.second.z;
        }
    }
}

bool Object::isShaderEnabled() const { return _isShaderEnabled; }

std::optional<std::array<std::size_t, 3>> Object::getFirstRayIntersection(
    const glm::vec3 rayOrig, const glm::vec3 rayDir) const
{
    std::vector<std::pair<std::array<std::size_t, 3>, float>>
        idsWithDistance {};

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
        if (Sculptor::intersectRayTriangleGLM(
                rayOrig, rayDir, triangleVertices, tuv)
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

glm::vec3
Object::getFaceNormalCross(std::array<std::size_t, 3> verticesId) const
{
    const auto vec4Length = glm::vec4::length();

    std::array<glm::vec3, 3> vertices {};

    for (int i = 0; i < verticesId.size(); ++i) {
        vertices.at(i) = { trVertices.at(verticesId.at(i) * vec4Length),
                           trVertices.at(verticesId.at(i) * vec4Length + 1),
                           trVertices.at(verticesId.at(i) * vec4Length + 2) };
    }

    const glm::vec3 edge1 { vertices[1] - vertices[0] };
    const glm::vec3 edge2 { vertices[2] - vertices[0] };

    return glm::normalize(glm::cross(edge2, edge1));
}

glm::vec3
Object::getFaceNormalAverage(std::array<std::size_t, 3> verticesId) const
{
    const auto vec3Length = glm::vec3::length();

    std::array<glm::vec3, 3> normals {};

    for (int i = 0; i < verticesId.size(); ++i) {
        normals.at(i) = { trNVertices.at(verticesId.at(i) * vec3Length),
                          trNVertices.at(verticesId.at(i) * vec3Length + 1),
                          trNVertices.at(verticesId.at(i) * vec3Length + 2) };
    }

    return (normals[0] + normals[1] + normals[2])
        / static_cast<float>(vec3Length);
}

std::string_view Object::getName() const { return name; }
