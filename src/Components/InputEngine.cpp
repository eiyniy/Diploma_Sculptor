#include <InputEngine.hpp>

#include <Camera.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <utility>

InputEngine::InputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
{
}

void InputEngine::update(const GLfloat dt)
{
    const auto keys = mainWindow->cGetKeys();
    const auto coordOffset = mainWindow->resetCoordOffset();

    if (keys[GLFW_KEY_ESCAPE]) {
        mainWindow->close();
    }

    if (keys[GLFW_KEY_W]) {
        camera->move(AxisName::Z, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_S]) {
        camera->move(AxisName::Z, Direction::Backward, dt);
    }
    if (keys[GLFW_KEY_D]) {
        camera->move(AxisName::X, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_A]) {
        camera->move(AxisName::X, Direction::Backward, dt);
    }
    if (keys[GLFW_KEY_SPACE]) {
        camera->move(AxisName::Y, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        camera->move(AxisName::Y, Direction::Backward, dt);
    }

    camera->rotate(coordOffset);
}