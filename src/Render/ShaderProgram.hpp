#pragma once

#include "ShaderAttribute.hpp"
#include <GL/glew.h>

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

const size_t infoLogSize = 512;

class ShaderProgram {
private:
    GLuint shaderProgram;

    std::string name;

    GLint success;

    // TODO: Replace with dynamic array
    std::array<GLchar, infoLogSize> infoLog;

    std::vector<GLuint> shaders;

    std::vector<ShaderAttribute> attributes;

    GLsizei attributesStride;

    bool _isUsed;

public:
    static const std::string defaultModelUniformName;
    static const std::string defaultViewUniformName;
    static const std::string defaultProjectionUniformName;

    ShaderProgram(std::string_view _name);

    void addShader(std::string sourcePath, GLenum shaderType);

    void addAttribute(const ShaderAttribute& attribute);

    void setupAttributes();

    void link();

    void enable();

    void disable();

    [[nodiscard]] inline GLuint get() const;

    [[nodiscard]] inline std::string_view getName() const;

    [[nodiscard]] inline bool isEnabled() const;
};

inline GLuint ShaderProgram::get() const { return shaderProgram; }

inline std::string_view ShaderProgram::getName() const { return name; }

inline bool ShaderProgram::isEnabled() const { return _isUsed; }
