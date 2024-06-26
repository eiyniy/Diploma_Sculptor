#pragma once

#include <IBuilder.hpp>
#include <Object.hpp>
#include <ObjectEntry.hpp>
#include <Triangle.hpp>

#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class ShaderProgram;
class Texture;

struct TransformEntryResult {
    TransformEntryResult(
        const ObjectEntry& entry,
        std::size_t id,
        std::size_t hash,
        std::size_t vertexId);

    ObjectEntry entry;
    std::size_t id;
    std::size_t hash;
    std::size_t vertexId;
};

class ObjectBuilder : public IBuilder<Object> {
private:
    bool isInited;
    bool isTrianglesAdded;
    bool isLinesAdded;
    bool isTransformed;
    bool isVAOSetup;

    std::unique_ptr<std::vector<glm::vec4>> vertices;
    std::unique_ptr<std::vector<glm::vec3>> nVertices;
    std::unique_ptr<std::vector<glm::vec2>> tVertices;

    std::unique_ptr<std::vector<Triangle>> triangles;
    std::unique_ptr<std::vector<glm::vec<2, GLuint>>> lines;

    std::unique_ptr<std::vector<glm::vec<3, GLuint>>> indices;

    std::unordered_map<std::size_t, std::vector<std::size_t>>
        vericesIdToIndicesId;
    std::unordered_map<std::size_t, std::size_t> indicesIdToVertexId;

    // TODO: Move this to IBuilder
    void throwIfNotInited(const std::string& message) const;

    void reset() override;

    [[nodiscard]] static std::size_t hashEntryIdsNaive(
        int vertexId,
        std::optional<int> tVertexId = std::nullopt,
        std::optional<int> nVertexId = std::nullopt);

    [[nodiscard]] static std::size_t hashEntryIdsRotl(
        std::size_t vertexId,
        std::optional<std::size_t> tVertexId = std::nullopt,
        std::optional<std::size_t> nVertexId = std::nullopt);

    [[nodiscard]] bool isShaderProgramFinished() const;

    [[nodiscard]] bool isFinished() const override;

    void transformTriangles();

    void transformLines();

public:
    ObjectBuilder();

    ObjectBuilder(const ObjectBuilder&) = delete;
    ObjectBuilder(ObjectBuilder&&) = default;
    ObjectBuilder& operator=(const ObjectBuilder&) = delete;
    ObjectBuilder& operator=(ObjectBuilder&&) = delete;

    virtual ~ObjectBuilder() = default;

    void init(
        std::string_view name,
        GLenum drawMode,
        std::unique_ptr<std::vector<glm::vec4>> _vertices);

    void addTriangles(std::unique_ptr<std::vector<Triangle>> _triangles);

    void addLines(std::unique_ptr<std::vector<glm::vec<2, GLuint>>> _lines);

    void addTVertices(std::unique_ptr<std::vector<glm::vec2>> _tVertices);

    void addNVertices(std::unique_ptr<std::vector<glm::vec3>> _nVertices);

    void addShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);

    void addTexture(std::unique_ptr<Texture> texture);

    void transform();

    void setupVAO();
};