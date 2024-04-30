#include <ViewState.hpp>

#include <BaseState.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <memory>

ViewState::ViewState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera,
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _projectionMat)
    : BaseState(
          std::make_unique<ViewInputEngine>(std::move(_eventBus)),
          std::make_unique<ModelRenderEngine>(
              std::move(_mainWindow),
              std::move(_camera),
              std::move(_modelMat),
              std::move(_projectionMat)))
{
}

StateType ViewState::getType() const { return StateType::View; }
