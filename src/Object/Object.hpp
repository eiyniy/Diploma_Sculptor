#pragma once

#include <Concepts.hpp>
#include <ShaderProgram.hpp> // IWYU pragma: keep
#include <Texture.hpp>

#include <vector_float3.hpp>

#include <GL/glew.h>

#include <array>
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <class T> class ConstructorPasskey;

class Object {

    friend class ObjectBuilder;

private:
    std::string name;

    GLenum drawMode;

    GLuint VAO;
    GLuint EBO;

    GLuint verticesVBO;
    GLuint tVerticesVBO;
    GLuint nVerticesVBO;

    std::shared_ptr<ShaderProgram> shaderProgram;

    bool _isShaderEnabled;

    std::map<std::size_t, std::vector<std::size_t>> connectedIndicesIds;

    std::vector<GLfloat> trVertices;
    std::vector<GLfloat> trTVertices;
    std::vector<GLfloat> trNVertices;

    std::vector<GLuint> indices;

    std::map<const std::string_view, std::unique_ptr<Texture>> textures;

    void throwIfShaderNotEnabled(const std::string& message) const;

    template <class T>
        requires IsUniformType<T>
    void loadUniform(std::string_view name, const T& value) const;

public:
    Object(ConstructorPasskey<Object>&& passkey);

    Object(const Object&) = default;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    ~Object();

    [[nodiscard]] bool isShaderEnabled() const;

    void bindTextures();

    void bindVerticesVBO();

    void bindTVerticesVBO();

    void bindNVerticesVBO();

    void bindEBO();

    void unbindVBO();

    void enableShader();

    void disableShader();

    void draw() const;

    void loadUniforms();

    void performTransform(
        const std::vector<std::pair<std::size_t, glm::vec3>>& transform);

    [[nodiscard]] std::optional<std::array<std::size_t, 3>>
    getFirstRayIntersection(glm::vec3 rayOrig, glm::vec3 rayDir) const;

    [[nodiscard]] glm::vec3
    getFaceNormalCross(std::array<std::size_t, 3> verticesId) const;

    [[nodiscard]] glm::vec3
    getFaceNormalAverage(std::array<std::size_t, 3> verticesId) const;

    [[nodiscard]] std::string_view getName() const;
};

template <class T>
    requires IsUniformType<T>
void Object::loadUniform(const std::string_view name, const T& value) const
{
    throwIfShaderNotEnabled("loadUniform");

    shaderProgram->loadUniform(name, value);
}
