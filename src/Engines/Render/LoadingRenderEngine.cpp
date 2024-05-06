#include <LoadingRenderEngine.hpp>

#include <MainWindow.hpp>

#include <utility>

LoadingRenderEngine::LoadingRenderEngine(
    std::shared_ptr<MainWindow> _mainWindow)
    : mainWindow(std::move(_mainWindow))
{
}

void LoadingRenderEngine::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    mainWindow->clear();
    mainWindow->swapBuffers();
}
