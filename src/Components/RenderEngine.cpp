#include <RenderEngine.hpp>

#include <Camera.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <matrix_clip_space.hpp>
#include <type_mat4x4.hpp>
#include <type_ptr.hpp>
#include <type_vec4.hpp>

#include <utility>

RenderEngine::RenderEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , modelMat(1)
    , viewMat(1)
    , projectionMat(1)
{
    projectionMat = glm::perspective(
        camera->cGetFOV(),
        mainWindow->getAspect(),
        Settings::get()->getZNear(),
        Settings::get()->getZFar());
};

void RenderEngine::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    MainWindow::clear();

    viewMat = camera->cGetViewMat();

    for (auto&& object : objects) {
        object.second->enableShader();

        object.second->loadUniform(
            ShaderProgram::defaultModelUniformName, modelMat);
        object.second->loadUniform(
            ShaderProgram::defaultViewUniformName, viewMat);
        object.second->loadUniform(
            ShaderProgram::defaultProjectionUniformName, projectionMat);

        object.second->draw();
        object.second->disableShader();
    }

    mainWindow->swapBuffers();
}

bool RenderEngine::shouldClose() const { return mainWindow->shouldClose(); }
