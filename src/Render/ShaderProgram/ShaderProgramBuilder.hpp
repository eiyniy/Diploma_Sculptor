#pragma once

#include <GL/glew.h>

#include <memory>
#include <set>
#include <string_view>

class ShaderProgram;
class ShaderAttribute;

class ShaderProgramBuilder {
private:
    std::unique_ptr<ShaderProgram> shaderProgram;

    bool isLinked;

    unsigned addedAttributesCount;

    std::set<GLenum> addedShadersTypes;

    void reset();

    [[nodiscard]] bool isAttributesFinished() const;

    [[nodiscard]] bool isShadersFinished() const;

public:
    ShaderProgramBuilder();

    void create(std::string_view name);

    std::unique_ptr<ShaderProgram> build();

    void addShader(std::string sourcePath, GLenum shaderType);

    void addAttribute(const ShaderAttribute& attribute);

    void link();
};