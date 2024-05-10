#include <LoadingUniformLoader.hpp>

#include <IUniformLoader.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>

#include <utility>

LoadingUniformLoader::LoadingUniformLoader(
    std::shared_ptr<MainWindow> _mainWindow)
    : mainWindow(std::move(_mainWindow))
{
}

void LoadingUniformLoader::load(ShaderProgram* shaderProgram)
{
    const auto resolution = mainWindow->getActiveResolution();
    shaderProgram->loadUniform(
        ShaderProgram::resolutionUName,
        glm::vec2(resolution.first, resolution.second));

    shaderProgram->loadUniform(
        ShaderProgram::timeUName, static_cast<float>(glfwGetTime()));
}
