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

using namespace std::string_view_literals;

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
    static constexpr auto modelUName = "uModel"sv;
    static constexpr auto viewUName = "uView"sv;
    static constexpr auto projectionUName = "uProjection"sv;

    static constexpr auto colorUName = "uColor"sv;

    static constexpr auto cameraPosUName = "uCameraPos"sv;

    static constexpr auto lightColorUName = "uLightColor"sv;

    static constexpr auto dimmingFactorUName = "uDimmingFactor"sv;
    static constexpr auto isDistanceDimmingUName = "uIsDistanceDimming"sv;

    static constexpr auto timeUName = "uTime"sv;
    static constexpr auto resolutionUName = "uResolution"sv;

    static constexpr auto positionAName = "position"sv;
    static constexpr auto texCoordAName = "texCoord"sv;
    static constexpr auto normalAName = "normal"sv;

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
