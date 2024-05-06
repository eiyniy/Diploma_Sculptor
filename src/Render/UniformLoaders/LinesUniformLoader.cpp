#include <LinesUniformLoader.hpp>

#include <BaseUniformLoader.hpp>
#include <IUniformLoader.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <utility>

LinesUniformLoader::LinesUniformLoader(
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _viewMat,
    std::shared_ptr<glm::mat4> _projectionMat)
    : BaseUniformLoader(
          std::move(_modelMat), std::move(_viewMat), std::move(_projectionMat))
{
}

void LinesUniformLoader::load(ShaderProgram* shaderProgram)
{
    BaseUniformLoader::load(shaderProgram);

    shaderProgram->loadUniform(
        ShaderProgram::colorUName, Settings::get()->getLinesColor());
}
