#include <ModelRenderEngine.hpp>

#include <Camera.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <matrix_clip_space.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_ptr.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>

#include <utility>

ModelRenderEngine::ModelRenderEngine(
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera,
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _projectionMat)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , modelMat(std::move(_modelMat))
    , projectionMat(std::move(_projectionMat)) {};

void ModelRenderEngine::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    mainWindow->clear();

    const auto viewMat = camera->cGetViewMat();

    for (auto&& object : objects) {
        object.second->enableShader();

        object.second->loadUniform(ShaderProgram::modelUName, *modelMat);
        object.second->loadUniform(ShaderProgram::viewUName, viewMat);
        object.second->loadUniform(
            ShaderProgram::projectionUName, *projectionMat);

        object.second->loadUniform(
            ShaderProgram::cameraPosUName, camera->cGetPos());

        object.second->loadUniform(
            ShaderProgram::lightColorUName, glm::vec3(1, 1, 1));

        object.second->loadUniform(ShaderProgram::dimmingFactorUName, 0.03F);
        object.second->loadUniform(ShaderProgram::isDistanceDimmingUName, true);

        object.second->draw();
        object.second->disableShader();
    }

    mainWindow->swapBuffers();
}

bool ModelRenderEngine::shouldClose() const
{
    return mainWindow->shouldClose();
}
