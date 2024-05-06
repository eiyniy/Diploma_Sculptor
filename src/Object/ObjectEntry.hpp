#pragma once

#include <qualifier.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <cstddef>
#include <optional>

class ObjectEntry {
private:
    glm::vec4 vertex;
    std::optional<glm::vec3> nVertex;
    std::optional<glm::vec2> tVertex;

public:
    ObjectEntry();
    ObjectEntry(
        const glm::vec4& _vertex,
        const std::optional<glm::vec3>& _nVertex = std::nullopt,
        const std::optional<glm::vec2>& _tVertex = std::nullopt);

    [[nodiscard]] std::size_t hash() const;

    bool operator==(const ObjectEntry& entry) const;

    struct HashFunction {
        std::size_t operator()(const ObjectEntry& entry) const;
    };

    [[nodiscard]] glm::vec4 getVertex() const;

    [[nodiscard]] std::optional<glm::vec3> getNVertex() const;

    [[nodiscard]] std::optional<glm::vec2> getTVertex() const;
};
