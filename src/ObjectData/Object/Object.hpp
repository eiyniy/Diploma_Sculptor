#pragma once

#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <matrix_float4x4.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

template <class T> class ConstructorPasskey;

class Object {

    friend class ObjectBuilder;

private:
    GLuint VBO, VAO, EBO;

    std::map<const std::string_view, std::unique_ptr<ShaderProgram>>
        shaderPrograms;

    std::string currentShaderProgramName;

    bool _isAnyShaderEnabled;

    bool _hasColor;
    bool _hasTexture;
    bool _hasIndices;

    std::vector<GLfloat> verticesUnion;
    std::size_t verticesSize;
    std::size_t verticesUnionSize;
    GLsizei verticesUnionStep;

    std::vector<GLuint> indicesUnion;
    std::size_t indicesUnionSize;

    std::map<const std::string_view, std::unique_ptr<Texture>> textures;

    [[nodiscard]] GLint findUniform(std::string_view uniformName) const;

    void throwIfShaderNotEnabled(const std::string& message) const;

    void throwIfShaderNotSelected(const std::string& message) const;

public:
    Object(ConstructorPasskey<Object>&& passkey);

    Object(const Object&) = default;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    ~Object();

    [[nodiscard]] bool hasIndices() const;

    [[nodiscard]] bool hasColor() const;

    [[nodiscard]] bool hasTexture() const;

    [[nodiscard]] bool isAnyShaderEnabled() const;

    void bindTexture(std::string_view name);

    void bindTextures();

    void selectShaderProgram(std::string_view name);

    void enableShader();

    void disableShader();

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
    return _hasTexture && !textures.empty();
}

inline bool Object::isAnyShaderEnabled() const { return _isAnyShaderEnabled; }