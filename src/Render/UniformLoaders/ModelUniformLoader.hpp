#pragma once

#include <BaseUniformLoader.hpp>

#include <matrix_float4x4.hpp>
#include <vector_float3.hpp>

#include <memory>

class Camera;
class ShaderProgram;

class ModelUniformLoader : public BaseUniformLoader {
private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<glm::vec3> lightColor;

public:
    ModelUniformLoader(
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _viewMat,
        std::shared_ptr<glm::mat4> _projectionMat,
        std::shared_ptr<Camera> _camera);

    void load(ShaderProgram* shaderProgram) override;
};
