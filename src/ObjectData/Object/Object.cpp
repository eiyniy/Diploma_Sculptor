#include <Object.hpp>

#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

Object::Object(ConstructorPasskey<Object>&& passkey)
    : trVerticesSize(0)
    , trTVerticesSize(0)
    , trNVerticesSize(0)
    , indicesSize(0)
    , VAO(0)
    , verticesVBO(0)
    , tVerticesVBO(0)
    , nVerticesVBO(0)
    , EBO(0)
    , _isAnyShaderEnabled(false)
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
    if (!isAnyShaderEnabled()) {
        throw std::logic_error("No one shader is enabled. " + message);
    }
}

void Object::throwIfShaderNotSelected(const std::string& message) const
{
    if (currentShaderProgramName.empty()) {
        throw std::logic_error("No one shader is selected. " + message);
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

void Object::selectShaderProgram(const std::string_view name)
{
    currentShaderProgramName = name;
}

void Object::enableShader()
{
    if (isAnyShaderEnabled()) {
        throw std::logic_error("Can't enable shader program. There is another "
                               "one enabled already");
    }

    throwIfShaderNotSelected("Can't enable shader program.");

    auto& shaderProgram = shaderPrograms.at(currentShaderProgramName);

    shaderProgram->enable();

    currentShaderProgramName = shaderProgram->getName();
    _isAnyShaderEnabled = true;
}

void Object::disableShader()
{
    if (!isAnyShaderEnabled()) {
        throw std::logic_error(
            "Can't disable shader program. No one of them is enabled");
    }

    shaderPrograms.at(currentShaderProgramName)->disable();
    _isAnyShaderEnabled = false;
}

void Object::draw() const
{
    throwIfShaderNotEnabled("draw");

    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indicesSize),
        GL_UNSIGNED_INT,
        nullptr);

    glBindVertexArray(0);
}

bool Object::isAnyShaderEnabled() const { return _isAnyShaderEnabled; }
