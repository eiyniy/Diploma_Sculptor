#pragma once

#include <BaseInputEngine.hpp>

class IEvent;

class LoadingInputEngine : public BaseInputEngine {
public:
    LoadingInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus);

    std::optional<StateType> update(float dt) override;
};
