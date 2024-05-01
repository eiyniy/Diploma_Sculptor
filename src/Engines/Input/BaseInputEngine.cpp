#include <BaseInputEngine.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

class IEvent;

BaseInputEngine::BaseInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus)
    : eventBus(std::move(_eventBus))
    , keys()
    , _isMouseMoved(true)
{
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

void BaseInputEngine::mouseMoveCallbackInner(double xpos, double ypos)
{
    mousePos = { xpos, ypos };
    _isMouseMoved = true;
}

void BaseInputEngine::mouseButtonCallbackInner(
    GLFWwindow* window, int button, int action, int mods)
{
}

const std::array<bool, keysLength>& BaseInputEngine::getKeys() const
{
    return keys;
}

std::pair<float, float> BaseInputEngine::getMousePos() const
{
    return mousePos;
}

bool& BaseInputEngine::isMouseMoved() { return _isMouseMoved; }

void BaseInputEngine::pushEvent(std::unique_ptr<IEvent> event)
{
    eventBus->push(std::move(event));
}

std::shared_ptr<std::queue<std::unique_ptr<IEvent>>>
BaseInputEngine::transmitEventBus()
{
    return std::move(eventBus);
}
