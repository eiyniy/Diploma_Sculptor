#include <ShaderUniform.hpp>

#include <stdexcept>

ShaderUniform::ShaderUniform(
    const std::string_view _name, const GLuint shaderProgram)
    : name(_name)
    , location(-1)
    , isLoaded(false)
{
    const auto uniformLocation
        = glGetUniformLocation(shaderProgram, name.data());

    if (uniformLocation == -1) {
        std::string message { "Can't find uniform location. Uniform name - " };
        message += name;

        throw std::logic_error(message);
    }

    location = uniformLocation;
}

std::string_view ShaderUniform::getName() const { return name; }

GLint ShaderUniform::getLocation() const { return location; }
