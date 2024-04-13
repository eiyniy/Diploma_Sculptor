#pragma once

#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <GL/glew.h>

#include <map>
#include <optional>
#include <string>
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
    // TODO: Move texture name to its fields
    std::optional<std::map<std::string, Texture>> textures;

    [[nodiscard]] GLint findUniform(const std::string& uniformName) const;

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

    void addTexture(const Texture& texture, const std::string& name);

    void bindTexture(const std::string& name);

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
