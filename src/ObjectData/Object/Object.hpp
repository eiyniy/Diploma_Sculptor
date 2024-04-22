#pragma once

#include <ShaderProgram.hpp>
#include <ShaderUniform.hpp>
#include <Texture.hpp>

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
    GLuint VAO;
    GLuint EBO;

    GLuint verticesVBO;
    GLuint tVerticesVBO;

    std::map<const std::string_view, std::unique_ptr<ShaderProgram>>
        shaderPrograms;

    std::string currentShaderProgramName;

    bool _isAnyShaderEnabled;

    // TODO: Rename (tr?)
    std::vector<GLfloat> trVertices;
    std::size_t trVerticesSize;
    GLsizei trVerticesStep;

    std::vector<GLfloat> trTVertices;
    std::size_t trTVerticesSize;
    GLsizei trTVerticesStep;

    std::vector<GLuint> indices;
    std::size_t indicesSize;

    std::map<const std::string_view, std::unique_ptr<Texture>> textures;

    void throwIfShaderNotEnabled(const std::string& message) const;

    void throwIfShaderNotSelected(const std::string& message) const;

public:
    Object(ConstructorPasskey<Object>&& passkey);

    Object(const Object&) = default;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    ~Object();

    [[nodiscard]] bool isAnyShaderEnabled() const;

    void bindTextures();

    void selectShaderProgram(std::string_view name);

    void enableShader();

    void disableShader();

    // TODO: Get rid of this chain of calls to loadUniform()
    template <class T>
        requires IsUniformType<T>
    void loadUniform(std::string_view name, const T& value) const;

    void draw() const;
};

template <class T>
    requires IsUniformType<T>
void Object::loadUniform(const std::string_view name, const T& value) const
{
    throwIfShaderNotEnabled("loadUniform");

    shaderPrograms.at(currentShaderProgramName)->loadUniform(name, value);
}
