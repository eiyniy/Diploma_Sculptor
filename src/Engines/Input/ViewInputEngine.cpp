#include <ViewInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <Camera.hpp>
#include <EditInputEngine.hpp>
#include <EditState.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>
#include <ModelInputEngine.hpp>
#include <ModelRenderEngine.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <utility>

ViewInputEngine::ViewInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : ModelInputEngine(std::move(_mainWindow), std::move(_camera))
{
    getMainWindow()->captureMouse();

    double cursorPosX = NAN;
    double cursorPosY = NAN;
    glfwGetCursorPos(getMainWindow()->get(), &cursorPosX, &cursorPosY);
    mousePos = { cursorPosX, cursorPosY };

    lastMousePos = mousePos;
}

void ViewInputEngine::mouseCallbackInner(double xpos, double ypos)
{
    mousePos = { xpos, ypos };
}

std::unique_ptr<BaseState> ViewInputEngine::update(const float dt)
{
    auto res = ModelInputEngine::update(dt);
    if (res != nullptr) {
        return std::move(res);
    }

    const std::pair<GLfloat, GLfloat> coordOffset
        = { mousePos.first - lastMousePos.first,
            lastMousePos.second - mousePos.second };
    lastMousePos = mousePos;

    const auto& keys = cGetKeys();

    if (keys[GLFW_KEY_W]) {
        getCamera()->move(AxisName::Z, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_S]) {
        getCamera()->move(AxisName::Z, Direction::Backward, dt);
    }
    if (keys[GLFW_KEY_D]) {
        getCamera()->move(AxisName::X, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_A]) {
        getCamera()->move(AxisName::X, Direction::Backward, dt);
    }
    if (keys[GLFW_KEY_SPACE]) {
        getCamera()->move(AxisName::Y, Direction::Forward, dt);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        getCamera()->move(AxisName::Y, Direction::Backward, dt);
    }

    if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_M]
        && isKeysDelayElapsed()) {
        return std::make_unique<EditState>(getMainWindow(), getCamera());
    }

    getCamera()->rotate(coordOffset);

    return nullptr;
}