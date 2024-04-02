#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <ShaderProgramm.hpp>

#include <BaseTextParser.hpp>

#include <stddef.h>
#include <GL/glew.h>

#include <memory>
#include <iostream>
#include <stdexcept>

ShaderProgram::ShaderProgram()
{
    shaderProgram = glCreateProgram();
}

void ShaderProgram::addShader(
    const std::string &sourcePath,
    GLenum shaderType)
{
    BaseTextParser parser{sourcePath};
    const GLchar *source = parser.readFile()->c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for compile time errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED");
    }

    shaders.push_back(shader);
}

void ShaderProgram::link()
{
    for (auto &&shader : shaders)
        glAttachShader(shaderProgram, shader);

    glLinkProgram(shaderProgram);

    for (auto &&shader : shaders)
        glDeleteShader(shader);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    }
}

void ShaderProgram::use()
{
    glUseProgram(shaderProgram);
}
