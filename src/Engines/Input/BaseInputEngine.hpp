#pragma once

#include <Camera.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <optional>
#include <queue>

class BaseState;
class MainWindow;

static constexpr std::size_t keysLength = 1024;

class BaseInputEngine {
private:
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> eventBus;

    std::array<bool, keysLength> keys;

    std::pair<float, float> mousePos;
    bool _isMouseMoved;

public:
    BaseInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    BaseInputEngine(const BaseInputEngine&) = delete;
    BaseInputEngine(BaseInputEngine&&) = delete;
    BaseInputEngine& operator=(const BaseInputEngine&) = delete;
    BaseInputEngine& operator=(BaseInputEngine&&) = delete;

    virtual ~BaseInputEngine() = default;

    virtual std::optional<StateType> update(float dt) = 0;

    virtual void keyCallbackInner(int key, int scancode, int action, int mode);

    virtual void mouseMoveCallbackInner(double xpos, double ypos);

    virtual void mouseButtonCallbackInner(
        GLFWwindow* window, int button, int action, int mods);

    [[nodiscard]] const std::array<bool, keysLength>& getKeys() const;

    [[nodiscard]] std::pair<float, float> getMousePos() const;

    [[nodiscard]] bool& isMouseMoved();

    void pushEvent(std::unique_ptr<IEvent> event);

    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> transmitEventBus();
};