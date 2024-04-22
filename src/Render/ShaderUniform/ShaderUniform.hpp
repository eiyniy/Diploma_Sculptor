#pragma once

#include <Concepts.hpp>

#include <matrix_float4x4.hpp>
#include <type_ptr.hpp> // IWYU pragma: keep
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <stdexcept>
#include <string>
#include <string_view>

class ShaderUniform {
private:
    std::string name;

    GLint location;

    bool isLoaded;

    void findLocation(GLuint shaderProgram);

public:
    ShaderUniform(std::string_view _name, GLuint shaderProgram);

    ShaderUniform(const ShaderUniform&) = default;
    ShaderUniform(ShaderUniform&&) = delete;
    ShaderUniform& operator=(const ShaderUniform&) = default;
    ShaderUniform& operator=(ShaderUniform&&) = delete;

    ~ShaderUniform() = default;

    template <class T>
        requires IsUniformType<T>
    void load(T value) const;

    [[nodiscard]] std::string_view getName() const;

    [[nodiscard]] GLint getLocation() const;
};

template <class T>
    requires IsUniformType<T>
void ShaderUniform::load(T value) const
{
    if constexpr (std::same_as<T, glm::mat4x4>) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    } else if constexpr (std::same_as<T, glm::vec2>) {
        glUniform2fv(location, 1, glm::value_ptr(value));
    } else if constexpr (std::same_as<T, glm::vec3>) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    } else if constexpr (std::same_as<T, glm::vec4>) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    } else if constexpr (std::same_as<T, int>) {
        glUniform1i(location, value);
    } else {
        const std::string error
            = "Can't load uniform \"" + name + "\". It has incompatible type.";

        throw std::logic_error(error);
    }
}