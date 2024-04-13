#pragma once

#include <GL/glew.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

const size_t infoLogSize = 512;

class ShaderProgram {
private:
    GLuint shaderProgram;

    GLint success;
    std::array<GLchar, infoLogSize> infoLog;

    std::vector<GLuint> shaders;

    bool _isUsed;

public:
    static const std::string defaultModelUniformName;
    static const std::string defaultViewUniformName;
    static const std::string defaultProjectionUniformName;

    ShaderProgram();

    void addShader(std::string sourcePath, GLenum shaderType);

    void link();

    void enable();
    void disable();

    [[nodiscard]] inline GLuint get() const;
    [[nodiscard]] inline bool isEnabled() const;
};

inline GLuint ShaderProgram::get() const { return shaderProgram; }
inline bool ShaderProgram::isEnabled() const { return _isUsed; }
