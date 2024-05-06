#include <LoadingState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <BaseState.hpp>
#include <Enums.hpp>
#include <LoadingInputEngine.hpp>
#include <LoadingRenderEngine.hpp>

#include <memory>
#include <utility>

class IEvent;
class MainWindow;

LoadingState::LoadingState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow)
    : BaseState(
          std::make_unique<LoadingInputEngine>(std::move(_eventBus)),
          std::make_unique<LoadingRenderEngine>(std::move(_mainWindow)))
{
}

StateType LoadingState::getType() const { return StateType::Loading; }
