#pragma once

#include "vector_float4.hpp"
#include <IBuilder.hpp>
#include <Object.hpp>
#include <Triangle.hpp>

#include <cstddef>
#include <optional>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>

#include <memory>
#include <string_view>
#include <vector>

class ShaderProgram;
class Texture;

struct Entry {
    glm::vec4 vertex;
    std::optional<glm::vec2> tVertex;

    Entry();
    Entry(
        const glm::vec4& _vertex,
        const std::optional<glm::vec2>& _tVertex = std::nullopt);

    [[nodiscard]] size_t hash() const;

    bool operator==(const Entry& entry) const;

    struct HashFunction {
        size_t operator()(const Entry& entry) const;
    };
};

class ObjectBuilder : public IBuilder<Object> {
private:
    bool isInited;
    bool isTransformed;
    bool isVAOSetup;

    std::unique_ptr<std::vector<glm::vec4>> vertices;
    std::unique_ptr<std::vector<glm::vec2>> tVertices;
    std::unique_ptr<std::vector<Triangle>> triangles;
    std::unique_ptr<std::vector<glm::vec<3, GLuint>>> indices;

    // TODO: Move this to IBuilder
    void throwIfNotInited(const std::string& message) const;

    void reset() override;

    void generateBuffers();

    void transform();

    [[nodiscard]] bool isShaderProgramFinished() const;

    [[nodiscard]] bool isFinished() const override;

public:
    ObjectBuilder();

    ObjectBuilder(const ObjectBuilder&) = delete;
    ObjectBuilder(ObjectBuilder&&) = default;
    ObjectBuilder& operator=(const ObjectBuilder&) = delete;
    ObjectBuilder& operator=(ObjectBuilder&&) = delete;

    virtual ~ObjectBuilder() = default;

    void init(
        std::unique_ptr<std::vector<glm::vec4>> _vertices,
        std::unique_ptr<std::vector<Triangle>> _triangles);

    void addTVertices(std::unique_ptr<std::vector<glm::vec2>> _tVertices);

    void addShaderProgram(std::unique_ptr<ShaderProgram> shaderProgram);

    void addTexture(std::unique_ptr<Texture> texture);

    void selectShaderProgram(std::string_view name);

    // TODO: Rename
    void merge();

    void setupVAO();
};