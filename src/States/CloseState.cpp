#include <CloseState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <BaseState.hpp>
#include <Enums.hpp>
#include <LoadingRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <memory>
#include <utility>

class ShaderProgram;
class Camera;
class IEvent;
class MainWindow;

CloseState::CloseState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<ShaderProgram> loadingShaderProgram)
    : BaseState(
          std::make_unique<ViewInputEngine>(std::move(_eventBus)),
          std::make_unique<LoadingRenderEngine>(
              std::move(_mainWindow), std::move(loadingShaderProgram)))
{
}

StateType CloseState::getType() const { return StateType::Close; }