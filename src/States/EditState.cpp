#include <EditState.hpp>

#include <BaseState.hpp>
#include <EditInputEngine.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <memory>

EditState::EditState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera,
    std::shared_ptr<glm::mat4> _modelMat,
    std::shared_ptr<glm::mat4> _projectionMat)
    : BaseState(
          std::make_unique<EditInputEngine>(std::move(_eventBus)),
          std::make_unique<ModelRenderEngine>(
              std::move(_mainWindow),
              std::move(_camera),
              std::move(_modelMat),
              std::move(_projectionMat)))
{
}

StateType EditState::getType() const { return StateType::Edit; }
