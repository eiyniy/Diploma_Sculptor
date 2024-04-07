#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

class ShaderProgram
{
private:
    GLuint shaderProgram;

    GLint success;
    GLchar infoLog[512];

    std::vector<GLuint> shaders;

public:
    ShaderProgram();

    void addShader(
        const std::string &sourcePath,
        GLenum shaderType);

    void link();
    void use();

    inline GLuint get() const;
};

inline GLuint ShaderProgram::get() const
{
    return shaderProgram;
}
