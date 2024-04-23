#pragma once

#include <matrix_float4x4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <concepts>

template <typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template <class T>
concept IsUniformType = IsAnyOf<
    T,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4x4,
    int,
    float,
    bool>;

template <class T>
concept IsGlmVec = IsAnyOf<T, glm::vec2, glm::vec3, glm::vec4>;