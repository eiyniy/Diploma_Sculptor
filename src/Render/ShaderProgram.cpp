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

ShaderProgram::ShaderProgram()
    : shaderProgram(glCreateProgram())
    , success(0)
    , infoLog()
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

    // Check for compile time errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog.data());
        std::cout << infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED");
    }

    shaders.push_back(shader);
}

void ShaderProgram::link()
{
    for (auto&& shader : shaders) {
        glAttachShader(shaderProgram, shader);
    }

    glLinkProgram(shaderProgram);

    for (auto&& shader : shaders) {
        glDeleteShader(shader);
    }

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(
            shaderProgram, infoLogSize, nullptr, infoLog.data());
        std::cout << infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    }
}

void ShaderProgram::use() const { glUseProgram(shaderProgram); }
