#include <ShaderProgram.hpp>

#include <BaseTextParser.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

const std::string ShaderProgram::defaultModelUniformName = "model";
const std::string ShaderProgram::defaultViewUniformName = "view";
const std::string ShaderProgram::defaultProjectionUniformName = "projection";

ShaderProgram::ShaderProgram(const std::string_view _name)
    : shaderProgram(glCreateProgram())
    , success(0)
    , infoLog()
    , _isUsed(false)
    , name(_name)
    , attributesStride(0)
{
}

void ShaderProgram::addShader(std::string sourcePath, GLenum shaderType)
{
    BaseTextParser parser { std::move(sourcePath) };

    const std::string sourceStr = parser.readFile();
    const char* source = sourceStr.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog.data());
        std::cout << infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED");
    }

    shaders.push_back(shader);
}

void ShaderProgram::addAttribute(const ShaderAttribute& attribute)
{
    attributes.push_back(attribute);
    attributesStride += static_cast<GLsizei>(
        attribute.getElementsCount() * attribute.getSizeofElement());
}

void ShaderProgram::setupAttributes()
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

void ShaderProgram::link()
{
    for (auto&& shader : shaders) {
        glAttachShader(shaderProgram, shader);
    }

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(
            shaderProgram, infoLogSize, nullptr, infoLog.data());
        std::cout << infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    }

    for (auto&& shader : shaders) {
        glDeleteShader(shader);
    }
}

void ShaderProgram::enable()
{
    _isUsed = true;
    glUseProgram(shaderProgram);
}

void ShaderProgram::disable()
{
    _isUsed = false;
    glUseProgram(0);
}
