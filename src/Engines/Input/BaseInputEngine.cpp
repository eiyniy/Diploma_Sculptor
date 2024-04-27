#include <BaseInputEngine.hpp>

#include <BaseState.hpp>

#include <utility>

BaseInputEngine::BaseInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , keys()
{
}

std::unique_ptr<BaseState> BaseInputEngine::update(const float dt)
{
    keys = mainWindow->cGetKeys();
    return nullptr;
}

std::shared_ptr<MainWindow> BaseInputEngine::getMainWindow() const
{
    return mainWindow;
}

std::shared_ptr<Camera> BaseInputEngine::getCamera() const { return camera; }

const std::array<bool, MainWindow::keysLength>&
BaseInputEngine::cGetKeys() const
{
    return keys;
}
