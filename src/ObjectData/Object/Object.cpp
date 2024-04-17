#include <Object.hpp>

#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <matrix_float4x4.hpp>
#include <type_mat4x4.hpp>
#include <type_ptr.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

Object::Object(ConstructorPasskey<Object>&& passkey)
    : verticesUnionSize(0)
    , indicesUnionSize(0)
    , VAO(0)
    , VBO(0)
    , EBO(0)
    , _isAnyShaderEnabled(false)
    , _hasColor(false)
    , _hasIndices(false)
    , _hasTexture(false)
    , verticesSize(0)
    , verticesUnionStep(0)
{
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    if (hasIndices()) {
        glDeleteBuffers(1, &EBO);
    }
}

GLint Object::findUniform(const std::string_view uniformName) const
{
    throwIfShaderNotEnabled("findUniform");

    const auto& shaderProgram = shaderPrograms.at(currentShaderProgramName);

    const auto uniformLocation
        = glGetUniformLocation(shaderProgram->get(), uniformName.data());

    if (uniformLocation == -1) {
        std::string message { "Can't find uniform location. Uniform name - " };
        message += uniformName;

        throw std::logic_error(message);
    }

    return uniformLocation;
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

// TODO: Remove this and work with ShaderAttributes
void Object::loadTransformMatrices(
    const glm::mat4& modelMat,
    const glm::mat4& viewMat,
    const glm::mat4& projectionMat) const
{
    const auto modelLoc = findUniform(ShaderProgram::defaultModelUniformName);
    const auto viewLoc = findUniform(ShaderProgram::defaultViewUniformName);
    const auto projectionLoc
        = findUniform(ShaderProgram::defaultProjectionUniformName);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(
        projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));
}

void Object::bindTexture(const std::string_view name)
{
    throwIfShaderNotEnabled("bindTexture");

    textures.at(name)->bind();

    const auto location = findUniform(name);

    glUniform1i(location, textures.at(name)->getTextureBlock() - GL_TEXTURE0);
}

void Object::bindTextures()
{
    throwIfShaderNotEnabled("bindTextures");

    if (!hasTexture()) {
        throw std::logic_error("Can't bind Textures. Object doesn't has them");
    }

    for (auto&& texture : textures) {
        bindTexture(texture.first);
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

    if (hasIndices()) {
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(indicesUnionSize),
            GL_UNSIGNED_INT,
            nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesSize));
    }

    glBindVertexArray(0);
}
