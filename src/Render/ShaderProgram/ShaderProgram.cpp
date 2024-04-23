#include <ShaderProgram.hpp>

#include <IBuilder.hpp>
#include <ShaderAttribute.hpp>

#include <GL/glew.h>

#include <string>

ShaderProgram::ShaderProgram(ConstructorPasskey<ShaderProgram>&& passkey)
    : program(glCreateProgram())
    , success(0)
    , _isUsed(false)
{
    infoLog.resize(infoLogSize);
}

void ShaderProgram::init(std::string_view _name) { name = _name; }

void ShaderProgram::enableAttribute(const std::string_view name)
{
    auto& attribute = attributes.at(name);
    attribute->enable(0, nullptr);
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
