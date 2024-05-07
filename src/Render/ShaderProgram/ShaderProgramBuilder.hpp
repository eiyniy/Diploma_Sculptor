#pragma once

#include <IBuilder.hpp>
#include <ShaderProgram.hpp>

#include <GL/glew.h>

#include <memory>
#include <set>
#include <string>
#include <string_view>

class ShaderAttribute;
class IUniformLoader;

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
    ShaderProgramBuilder(ShaderProgramBuilder&& shaderProgramBuilder) = default;
    ShaderProgramBuilder& operator=(const ShaderProgramBuilder&) = delete;
    ShaderProgramBuilder& operator=(ShaderProgramBuilder&&) = default;

    virtual ~ShaderProgramBuilder() = default;

    void
    init(std::string_view name, std::unique_ptr<IUniformLoader> uniformLoader);

    void addShader(const std::string& sourcePath, GLenum shaderType);

    void addAttribute(std::unique_ptr<ShaderAttribute> attribute);

    void addNewUniform(std::string_view name);

    void link();
};