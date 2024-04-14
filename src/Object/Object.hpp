#pragma once

#include <ShaderProgram.hpp>
#include <Texture.hpp>

// #include <glm/fwd.hpp>
// #include <glm/glm.hpp>

#include <matrix_float4x4.hpp>
#include <memory>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class Object {
private:
    GLuint VBO, VAO, EBO;

    ShaderProgram shaderProgram;

    bool _hasColor;
    bool _hasTexture;
    bool _hasIndices;

    std::vector<GLfloat> verticesUnion;
    std::size_t verticesSize;
    std::size_t verticesUnionSize;
    GLsizei verticesUnionStep;

    std::vector<GLuint> indicesUnion;
    std::size_t indicesUnionSize;

    // TODO: Remove optional's from class fields
    std::optional<std::map<std::string_view, std::unique_ptr<Texture>>>
        textures;

    [[nodiscard]] GLint findUniform(std::string_view uniformName) const;

    void throwIfShaderNotUsed(const std::string& message) const;

public:
    Object(const Object&) = default;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    Object(
        const std::vector<glm::vec3>& _vertices,
        const std::optional<std::vector<glm::vec3>>& _colorVertices,
        const std::optional<std::vector<glm::vec2>>& _textureVertices,
        const std::optional<std::vector<glm::vec<3, GLuint>>>& _indices);

    ~Object();

    [[nodiscard]] bool hasIndices() const;
    [[nodiscard]] bool hasColor() const;
    [[nodiscard]] bool hasTexture() const;

    void addTexture(std::unique_ptr<Texture> texture);

    void bindTexture(std::string_view name);

    void bindTextures();

    void enableShader();
    void disableShader();

    void setupVAO();

    void loadTransformMatrices(
        const glm::mat4& modelMat,
        const glm::mat4& viewMat,
        const glm::mat4& projectionMat) const;

    void draw() const;
};

inline bool Object::hasIndices() const { return _hasIndices; }

inline bool Object::hasColor() const { return _hasColor; }

inline bool Object::hasTexture() const
{
    return _hasTexture && textures.has_value();
}
