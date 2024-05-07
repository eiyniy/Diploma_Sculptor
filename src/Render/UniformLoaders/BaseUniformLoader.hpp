#pragma once

#include <IUniformLoader.hpp>

#include <matrix_float4x4.hpp>

#include <memory>

class ShaderProgram;

class BaseUniformLoader : public IUniformLoader {
private:
    std::shared_ptr<glm::mat4> modelMat;
    std::shared_ptr<glm::mat4> viewMat;
    std::shared_ptr<glm::mat4> projectionMat;

public:
    BaseUniformLoader(
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _viewMat,
        std::shared_ptr<glm::mat4> _projectionMat);

    void load(ShaderProgram* shaderProgram) override;
};
