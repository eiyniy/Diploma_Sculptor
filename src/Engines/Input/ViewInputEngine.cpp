#include <ViewInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <CameraMoveEvent.hpp>
#include <CameraRotateEvent.hpp>
#include <CaptureMouseEvent.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>
#include <ModelInputEngine.hpp>

#include <GL/glew.h> // IWYU pragma: keep
#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <optional>
#include <utility>

ViewInputEngine::ViewInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus)
    : ModelInputEngine(std::move(_eventBus))
{
    pushEvent(std::make_unique<CaptureMouseEvent>());
}

void ViewInputEngine::mouseMoveCallbackInner(double xpos, double ypos)
{
    BaseInputEngine::mouseMoveCallbackInner(xpos, ypos);
}

std::optional<StateType> ViewInputEngine::update(const float dt)
{
    auto res = ModelInputEngine::update(dt);
    if (res.has_value()) {
        return res;
    }

    const auto& keys = getKeys();

    if (keys[GLFW_KEY_W]) {
        pushEvent(
            std::make_unique<CameraMoveEvent>(AxisName::Z, Direction::Forward));
    }
    if (keys[GLFW_KEY_S]) {
        pushEvent(std::make_unique<CameraMoveEvent>(
            AxisName::Z, Direction::Backward));
    }
    if (keys[GLFW_KEY_D]) {
        pushEvent(
            std::make_unique<CameraMoveEvent>(AxisName::X, Direction::Forward));
    }
    if (keys[GLFW_KEY_A]) {
        pushEvent(std::make_unique<CameraMoveEvent>(
            AxisName::X, Direction::Backward));
    }
    if (keys[GLFW_KEY_SPACE]) {
        pushEvent(
            std::make_unique<CameraMoveEvent>(AxisName::Y, Direction::Forward));
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        pushEvent(std::make_unique<CameraMoveEvent>(
            AxisName::Y, Direction::Backward));
    }

    if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_M]
        && isKeysDelayElapsed()) {
        return StateType::Edit;
    }

    if (keys[GLFW_KEY_UP]) {
        const std::pair<float, float> coordOffset { 0.F, 1.F };
        pushEvent(std::make_unique<CameraRotateEvent>(coordOffset));
    }
    if (keys[GLFW_KEY_DOWN]) {
        const std::pair<float, float> coordOffset { 0.F, -1.F };
        pushEvent(std::make_unique<CameraRotateEvent>(coordOffset));
    }
    if (keys[GLFW_KEY_LEFT]) {
        const std::pair<float, float> coordOffset { -1.F, 0.F };
        pushEvent(std::make_unique<CameraRotateEvent>(coordOffset));
    }
    if (keys[GLFW_KEY_RIGHT]) {
        const std::pair<float, float> coordOffset { 1.F, 0.F };
        pushEvent(std::make_unique<CameraRotateEvent>(coordOffset));
    }

    if (isMouseMoved()) {
        const std::pair<float, float> coordOffset {
            getMousePos().first - lastMousePos.first,
            lastMousePos.second - getMousePos().second
        };
        lastMousePos = getMousePos();

        pushEvent(std::make_unique<CameraRotateEvent>(coordOffset));

        isMouseMoved() = false;
    }

    return std::nullopt;
}