#include <ShaderProgram.hpp>

#include <IBuilder.hpp>
#include <ShaderAttribute.hpp>

#include <GL/glew.h>

#include <string>

const std::string ShaderProgram::defaultModelUniformName = "model";
const std::string ShaderProgram::defaultViewUniformName = "view";
const std::string ShaderProgram::defaultProjectionUniformName = "projection";

ShaderProgram::ShaderProgram(ConstructorPasskey<ShaderProgram>&& passkey)
    : program(glCreateProgram())
    , success(0)
    , _isUsed(false)
    , attributesStride(0)
{
    infoLog.resize(infoLogSize);
}

void ShaderProgram::init(std::string_view _name) { name = _name; }

void ShaderProgram::enableAttributes()
{
    auto offset = 0;
    for (auto&& attribute : attributes) {
        attribute.enable(
            attributesStride,
            // NOLINTNEXTLINE
            reinterpret_cast<GLvoid*>(offset * attribute.getSizeofElement()));

        offset += attribute.getElementsCount();
    }
}

void ShaderProgram::enable()
{
    _isUsed = true;
    glUseProgram(program);
}

void ShaderProgram::disable()
{
    _isUsed = false;
    glUseProgram(0);
}

GLuint ShaderProgram::get() const { return program; }

std::string_view ShaderProgram::getName() const { return name; }

bool ShaderProgram::isEnabled() const { return _isUsed; }
