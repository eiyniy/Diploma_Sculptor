#pragma once

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

const size_t infoLogSize = 512;

class ShaderProgram {

    friend class ShaderProgramBuilder;

private:
    void init(std::string_view _name);

    GLuint program;

    std::string name;

    GLint success;

    std::vector<GLchar> infoLog;

    std::vector<GLuint> shaders;

    // TODO: Replace with interface IShaderAttribute to get rid of #include
    std::vector<ShaderAttribute> attributes;

    std::map<const std::string_view, std::unique_ptr<ShaderUniform>> uniforms;

    GLsizei attributesStride;

    bool _isUsed;

public:
    static const std::string defaultModelUniformName;
    static const std::string defaultViewUniformName;
    static const std::string defaultProjectionUniformName;

    ShaderProgram(ConstructorPasskey<ShaderProgram>&& passkey);

    void enableAttributes();

    template <class T>
        requires IsUniformType<T>
    void loadUniform(std::string_view name, const T& value) const;

    void enable();

    void disable();

    [[nodiscard]] inline GLuint get() const;

    [[nodiscard]] inline std::string_view getName() const;

    [[nodiscard]] inline bool isEnabled() const;
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

inline GLuint ShaderProgram::get() const { return program; }

inline std::string_view ShaderProgram::getName() const { return name; }

inline bool ShaderProgram::isEnabled() const { return _isUsed; }
