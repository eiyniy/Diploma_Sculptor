#pragma once

#include <IBuilder.hpp>
#include <Object.hpp>
#include <Triangle.hpp>

#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class ShaderProgram;
class Texture;

class ObjectBuilder : public IBuilder<Object> {
private:
    bool isInited;
    bool isTransformed;
    bool isVAOSetup;

    std::unique_ptr<std::vector<glm::vec4>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> nVertices;
    std::unique_ptr<std::vector<glm::vec2>> tVertices;
    std::unique_ptr<std::vector<Triangle>> triangles;
    std::unique_ptr<std::vector<glm::vec<3, GLuint>>> indices;

    std::map<std::size_t, std::vector<std::size_t>> vericesIdToIndicesId;
    std::map<std::size_t, std::size_t> indicesIdToVertexId;

    // TODO: Move this to IBuilder
    void throwIfNotInited(const std::string& message) const;

    void reset() override;

    [[nodiscard]] static std::size_t hashEntryIdsNaive(
        int vertexId,
        std::optional<int> tVertexId = std::nullopt,
        std::optional<int> nVertexId = std::nullopt);

    [[nodiscard]] static std::size_t hashEntryIdsRotl(
        int vertexId,
        std::optional<int> tVertexId = std::nullopt,
        std::optional<int> nVertexId = std::nullopt);

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

    void addNVertices(std::unique_ptr<std::vector<glm::vec3>> _nVertices);

    void addShaderProgram(std::unique_ptr<ShaderProgram> shaderProgram);

    void addTexture(std::unique_ptr<Texture> texture);

    void selectShaderProgram(std::string_view name);

    void transform();

    void setupVAO();
};