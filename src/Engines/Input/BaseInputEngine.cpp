#include <BaseInputEngine.hpp>

#include <BaseState.hpp>
#include <MainWindow.hpp>

#include <utility>

BaseInputEngine::BaseInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , keys()
{
    mainWindow->setUserPointer(this);
}

void BaseInputEngine::keyCallbackInner(
    const int key, const int scancode, const int action, const int mode)
{
    if (key >= keys.size()) {
        throw std::out_of_range("Key code is out of range");
    }

    if (action == GLFW_PRESS) {
        keys.at(key) = true;
    } else if (action == GLFW_RELEASE) {
        keys.at(key) = false;
    }
}

std::shared_ptr<MainWindow> BaseInputEngine::getMainWindow() const
{
    return mainWindow;
}

std::shared_ptr<Camera> BaseInputEngine::getCamera() const { return camera; }

const std::array<bool, keysLength>& BaseInputEngine::cGetKeys() const
{
    return keys;
}
