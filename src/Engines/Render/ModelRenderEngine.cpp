#include <ModelRenderEngine.hpp>

#include <MainWindow.hpp>
#include <Object.hpp>

#include <utility>

ModelRenderEngine::ModelRenderEngine(
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera,
    std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>> _objects)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , objects(std::move(_objects)) {};

void ModelRenderEngine::draw()
{
    mainWindow->clear();

    for (auto&& object : *objects) {
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
