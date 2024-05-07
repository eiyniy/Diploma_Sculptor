#include <ModelRenderEngine.hpp>

#include <MainWindow.hpp>
#include <Object.hpp>

#include <utility>

ModelRenderEngine::ModelRenderEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera)) {};

void ModelRenderEngine::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    mainWindow->clear();

    for (auto&& object : objects) {
        object.second->enableShader();

        object.second->loadUniforms();

        object.second->draw();
        object.second->disableShader();
    }

    mainWindow->swapBuffers();
}

bool ModelRenderEngine::shouldClose() const
{
    return mainWindow->shouldClose();
}
