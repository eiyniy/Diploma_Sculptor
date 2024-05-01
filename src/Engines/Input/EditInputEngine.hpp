#pragma once

#include <ModelInputEngine.hpp>

#include <array>
#include <memory>

class MainWindow;
class Camera;

class EditInputEngine : public ModelInputEngine {
public:
    static constexpr std::size_t mouseButtonsCount = 8;

private:
    std::array<bool, mouseButtonsCount> mouse;

public:
    EditInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    std::optional<StateType> update(float dt) override;

    void mouseButtonCallbackInner(
        GLFWwindow* window, int button, int action, int mods) override;
};