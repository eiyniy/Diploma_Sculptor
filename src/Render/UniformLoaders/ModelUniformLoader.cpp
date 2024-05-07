#include <ModelUniformLoader.hpp>

#include <BaseUniformLoader.hpp>
#include <Camera.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_ptr.hpp>

#include <utility>

ModelUniformLoader::ModelUniformLoader(
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _viewMat,
    std::shared_ptr<glm::mat4> _projectionMat,
    std::shared_ptr<Camera> _camera)
    : BaseUniformLoader(
          std::move(_modelMat), std::move(_viewMat), std::move(_projectionMat))
    , camera(std::move(_camera))
{
}

void ModelUniformLoader::load(ShaderProgram* shaderProgram)
{
    BaseUniformLoader::load(shaderProgram);

    shaderProgram->loadUniform(
        ShaderProgram::cameraPosUName, camera->cGetPos());

    shaderProgram->loadUniform(
        ShaderProgram::lightColorUName, Settings::get()->getLightColor());

    shaderProgram->loadUniform(
        ShaderProgram::dimmingFactorUName,
        Settings::get()->getDistanceDimmingFactor());
    shaderProgram->loadUniform(
        ShaderProgram::isDistanceDimmingUName,
        Settings::get()->getIsDistanceDimmingEnabled());
}
