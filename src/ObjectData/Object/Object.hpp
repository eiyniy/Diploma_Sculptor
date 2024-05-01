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
    GLuint nVerticesVBO;

    std::map<const std::string_view, std::unique_ptr<ShaderProgram>>
        shaderPrograms;

    std::string currentShaderProgramName;

    bool _isAnyShaderEnabled;

    // TODO: Rename (tr?)
    // TODO: Make unique_ptr
    std::vector<GLfloat> trVertices;

    std::vector<GLfloat> trTVertices;

    std::vector<GLfloat> trNVertices;

    std::vector<GLuint> indices;

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

    void bindVerticesVBO();

    void bindTVerticesVBO();
    
    void bindNVerticesVBO();
    
    void bindIndicesEBO();

    void unbindVBO();

    void selectShaderProgram(std::string_view name);

    void enableShader();

    void disableShader();

    // TODO: Get rid of this chain of calls to loadUniform()
    template <class T>
        requires IsUniformType<T>
    void loadUniform(std::string_view name, const T& value) const;

    void draw() const;

    void performTransform(
        const std::vector<std::pair<std::size_t, glm::vec3>>& transform);

    [[nodiscard]] const std::vector<GLfloat>& getTrVertices() const;

    [[nodiscard]] const std::vector<GLuint>& getIndices() const;
};

template <class T>
    requires IsUniformType<T>
void Object::loadUniform(const std::string_view name, const T& value) const
{
    throwIfShaderNotEnabled("loadUniform");

    shaderPrograms.at(currentShaderProgramName)->loadUniform(name, value);
}
