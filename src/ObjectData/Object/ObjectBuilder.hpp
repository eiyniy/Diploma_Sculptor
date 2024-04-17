#pragma once

#include <IBuilder.hpp>
#include <Object.hpp>

#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>

#include <memory>
#include <string_view>
#include <vector>

class ShaderProgram;
class Texture;

class ObjectBuilder : public IBuilder<Object> {
private:
    bool isInited;
    bool isMerged;
    bool isVAOSetup;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec<3, GLuint>> indices;

    void reset() override;

    void generateBuffers();

    void calcVerticesUnionParams();

    void mergeVertices();

    void mergeIndices();

    [[nodiscard]] bool isShaderProgramFinished() const;

    [[nodiscard]] bool isFinished() const override;

public:
    ObjectBuilder();

    ObjectBuilder(const ObjectBuilder&) = delete;
    ObjectBuilder(ObjectBuilder&&) = default;
    ObjectBuilder& operator=(const ObjectBuilder&) = delete;
    ObjectBuilder& operator=(ObjectBuilder&&) = delete;

    virtual ~ObjectBuilder() = default;

    void init(const std::vector<glm::vec3>& _vertices);

    void addColors(const std::vector<glm::vec3>& _colors);

    void addTextureCoords(const std::vector<glm::vec2>& _textureCoords);

    void addIndices(const std::vector<glm::vec<3, GLuint>>& _indices);

    void addShaderProgram(std::unique_ptr<ShaderProgram> shaderProgram);

    void addTexture(std::unique_ptr<Texture> texture);

    void selectShaderProgram(std::string_view name);

    void merge();

    void setupVAO();
};