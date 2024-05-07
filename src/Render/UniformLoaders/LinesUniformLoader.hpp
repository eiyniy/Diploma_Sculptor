#pragma once

#include <BaseUniformLoader.hpp>

#include <matrix_float4x4.hpp>

#include <memory>

class ShaderProgram;

class LinesUniformLoader : public BaseUniformLoader {
public:
    LinesUniformLoader(
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _viewMat,
        std::shared_ptr<glm::mat4> _projectionMat);

    void load(ShaderProgram* shaderProgram) override;
};
