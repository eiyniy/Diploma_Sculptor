#include <LoadingState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <BaseState.hpp>
#include <Enums.hpp>
#include <LoadingInputEngine.hpp>
#include <LoadingRenderEngine.hpp>
#include <ShaderProgramManager.hpp>

#include <memory>
#include <utility>

class IEvent;
class MainWindow;

LoadingState::LoadingState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow,
    const std::shared_ptr<ShaderProgramManager>& _shaderProgramManager)
    : BaseState(
          std::make_unique<LoadingInputEngine>(
              std::move(_eventBus), _shaderProgramManager),
          std::make_unique<LoadingRenderEngine>(
              std::move(_mainWindow),
              _shaderProgramManager->getShaderProgram(
                  ShaderProgramManager::loadingSPName)))
{
}

StateType LoadingState::getType() const { return StateType::Loading; }
