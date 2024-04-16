#include <ShaderProgram.hpp>

#include <BaseTextParser.hpp>

#include <GL/glew.h>

#include <string>

const std::string ShaderProgram::defaultModelUniformName = "model";
const std::string ShaderProgram::defaultViewUniformName = "view";
const std::string ShaderProgram::defaultProjectionUniformName = "projection";

ShaderProgram::ShaderProgram(const std::string_view _name)
    : program(glCreateProgram())
    , success(0)
    , _isUsed(false)
    , name(_name)
    , attributesStride(0)
{
    infoLog.resize(infoLogSize);
}

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
