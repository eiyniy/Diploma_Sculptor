#include <ModelRenderEngine.hpp>

#include <Camera.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_ptr.hpp>
#include <type_vec3.hpp>
#include <vector_float3.hpp>

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
