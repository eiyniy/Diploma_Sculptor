#include "EngineLoadEvent.hpp"
#include "Enums.hpp"
#include <LoadingInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <IEvent.hpp>

#include <memory>
#include <optional>
#include <utility>

LoadingInputEngine::LoadingInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus)
    : BaseInputEngine(std::move(_eventBus))
{
    pushEvent(std::make_unique<EngineLoadEvent>());
}

std::optional<StateType> LoadingInputEngine::update(const float dt)
{
    // TODO: Add async load
    return StateType::View;
}
