#pragma once

#include <IBuilder.hpp>
#include <ShaderProgram.hpp>

#include <GL/glew.h>

#include <set>
#include <string>
#include <string_view>

class ShaderAttribute;

class ShaderProgramBuilder : public IBuilder<ShaderProgram> {
private:
    bool isInited;

    bool isLinked;

    std::set<GLenum> addedShadersTypes;

    void reset() override;

    [[nodiscard]] bool isFinished() const override;

    [[nodiscard]] bool isShadersFinished() const;

public:
    ShaderProgramBuilder();

    ShaderProgramBuilder(const ShaderProgramBuilder& shaderProgramBuilder)
        = delete;
    ShaderProgramBuilder(ShaderProgramBuilder&& shaderProgramBuilder) = delete;
    ShaderProgramBuilder& operator=(const ShaderProgramBuilder&) = delete;
    ShaderProgramBuilder& operator=(ShaderProgramBuilder&&) = delete;

    virtual ~ShaderProgramBuilder() = default;

    void init(std::string_view name);

    void addShader(std::string sourcePath, GLenum shaderType);

    void addAttribute(const ShaderAttribute& attribute);

    void addNewUniform(std::string_view name, GLuint program);

    void link();
};