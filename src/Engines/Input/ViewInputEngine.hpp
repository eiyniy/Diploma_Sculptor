#pragma once

#include <ModelInputEngine.hpp>

#include <memory>

class ViewInputEngine : public ModelInputEngine {
private:
    std::pair<float, float> lastMousePos;

    bool isMouseMoved;

public:
    ViewInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    std::optional<StateType> update(float dt) override;

    void mouseMoveCallbackInner(double xpos, double ypos) override;
};