#pragma once

#include <ModelInputEngine.hpp>

#include <memory>

class MainWindow;
class Camera;

class EditInputEngine : public ModelInputEngine {
public:
    EditInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    std::optional<StateType> update(float dt) override;

    void mouseButtonCallbackInner(
        GLFWwindow* window, int button, int action, int mods) override;
};