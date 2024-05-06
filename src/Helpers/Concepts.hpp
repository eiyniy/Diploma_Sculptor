#pragma once

#include <BaseInputEngine.hpp>

#include <limits>
#include <matrix_float4x4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <type_traits>

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

template <int L>
concept GlmVecLength = L > 0 && L <= 4;

template <typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

template <int L, typename V>
concept GlmVec = requires {
    GlmVecLength<L>;
    NumericType<V>;
};

template <class T>
concept InputEnginePt = std::is_pointer_v<T>
    && (std::same_as<BaseInputEngine, std::remove_pointer_t<T>>
        || std::is_base_of_v<BaseInputEngine, std::remove_pointer_t<T>>);

template <class T>
concept NotInteger = !std::numeric_limits<T>::is_integer;
