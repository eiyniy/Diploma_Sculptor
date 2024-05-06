#pragma once

#include <Concepts.hpp>
#include <IUniformLoader.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderUniform.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

template <class T> class ConstructorPasskey;

const std::size_t infoLogSize = 512;

class ShaderProgram {

    friend class ShaderProgramBuilder;

private:
    void init(std::string_view _name);

    GLuint program;

    // TODO: Remove name
    std::string name;

    GLint success;

    std::vector<GLchar> infoLog;

    std::vector<GLuint> shaders;

    std::map<const std::string_view, std::unique_ptr<ShaderAttribute>>
        attributes;

    std::map<const std::string_view, std::unique_ptr<ShaderUniform>> uniforms;

    std::unique_ptr<IUniformLoader> uniformLoader;

    bool _isUsed;

public:
    static constexpr std::string_view modelUName = "uModel";
    static constexpr std::string_view viewUName = "uView";
    static constexpr std::string_view projectionUName = "uProjection";

    static constexpr std::string_view colorUName = "uColor";

    static constexpr std::string_view cameraPosUName = "uCameraPos";

    static constexpr std::string_view lightColorUName = "uLightColor";

    static constexpr std::string_view dimmingFactorUName = "uDimmingFactor";
    static constexpr std::string_view isDistanceDimmingUName
        = "uIsDistanceDimming";

    static constexpr std::string_view positionAName = "position";
    static constexpr std::string_view texCoordAName = "texCoord";
    static constexpr std::string_view normalAName = "normal";

    ShaderProgram(ConstructorPasskey<ShaderProgram>&& passkey);

    void enableAttribute(std::string_view name);

    template <class T>
        requires IsUniformType<T>
    void loadUniform(std::string_view name, const T& value) const;

    void loadUniforms();

    void enable();

    void disable();

    [[nodiscard]] GLuint get() const;

    [[nodiscard]] std::string_view getName() const;

    [[nodiscard]] bool isEnabled() const;
};

template <class T>
    requires IsUniformType<T>
void ShaderProgram::loadUniform(
    const std::string_view name, const T& value) const
{
    if (!isEnabled()) {
        const std::string error = "Can't load uniform \"" + std::string(name)
            + "\". Shader program is disabled.";

        throw std::logic_error(error);
    }

    uniforms.at(name)->load(value);
}
