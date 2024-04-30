#include <EditInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>
#include <ReleaseMouseEvent.hpp>
#include <Sculptor.hpp>
#include <SculptorEditEvent.hpp>
#include <ViewState.hpp>

#include <GLFW/glfw3.h>

#include <memory>
#include <optional>
#include <utility>

EditInputEngine::EditInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus)
    : ModelInputEngine(std::move(_eventBus))
{
    pushEvent(std::make_unique<ReleaseMouseEvent>());
}

std::optional<StateType> EditInputEngine::update(const float dt)
{
    auto res = ModelInputEngine::update(dt);
    if (res.has_value()) {
        return res;
    }

    const auto& keys = getKeys();

    if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_M]
        && isKeysDelayElapsed()) {
        return StateType::View;
    }

    return std::nullopt;
}

void EditInputEngine::mouseButtonCallbackInner(
    GLFWwindow* window, const int button, const int action, const int mods)
{
    BaseInputEngine::mouseButtonCallbackInner(window, button, action, mods);

    // TODO: Move to update()
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        pushEvent(std::make_unique<SculptorEditEvent>(getMousePos()));
    }
}
