#pragma once

#include <Enums.hpp>
#include <ModelInputEngine.hpp>

#include <memory>
#include <optional>
#include <queue>
#include <utility>

class IEvent;

class ViewInputEngine : public ModelInputEngine {
private:
    std::pair<float, float> lastMousePos;

public:
    ViewInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    std::optional<StateType> update(float dt) override;

    void mouseMoveCallbackInner(double xpos, double ypos) override;
};