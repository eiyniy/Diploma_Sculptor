#include <ModelInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <CloseEvent.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <chrono>
#include <memory>
#include <utility>

ModelInputEngine::ModelInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus)
    : BaseInputEngine(std::move(_eventBus))
    , switchTime(std::chrono::high_resolution_clock::now())
{
}

std::optional<StateType> ModelInputEngine::update(const float dt)
{
    if (getKeys()[GLFW_KEY_ESCAPE]) {
        pushEvent(std::make_unique<CloseEvent>());
        return StateType::Close;
    }

    return std::nullopt;
}

bool ModelInputEngine::isKeysDelayElapsed() const
{
    return (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - switchTime))
               .count()
        > switchDelayMs;
}
