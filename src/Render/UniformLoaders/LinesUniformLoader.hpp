#pragma once

#include <BaseUniformLoader.hpp>
#include <IUniformLoader.hpp>

#include <vector_float3.hpp>

class LinesUniformLoader : public BaseUniformLoader {
public:
    LinesUniformLoader(
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _viewMat,
        std::shared_ptr<glm::mat4> _projectionMat);

    void load(ShaderProgram* shaderProgram) override;
};
