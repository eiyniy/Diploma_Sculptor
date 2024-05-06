#include <BaseUniformLoader.hpp>

#include <IUniformLoader.hpp>
#include <ShaderProgram.hpp>

#include <utility>

BaseUniformLoader::BaseUniformLoader(
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _viewMat,
    std::shared_ptr<glm::mat4> _projectionMat)
    : modelMat(std::move(_modelMat))
    , viewMat(std::move(_viewMat))
    , projectionMat(std::move(_projectionMat))
{
}

void BaseUniformLoader::load(ShaderProgram* shaderProgram)
{
    shaderProgram->loadUniform(ShaderProgram::modelUName, *modelMat);
    shaderProgram->loadUniform(ShaderProgram::viewUName, *viewMat);
    shaderProgram->loadUniform(ShaderProgram::projectionUName, *projectionMat);
}
