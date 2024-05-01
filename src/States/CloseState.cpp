#include <CloseState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <BaseState.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <matrix_float4x4.hpp>

#include <utility>

class Camera;
class IEvent;
class MainWindow;

CloseState::CloseState(
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

StateType CloseState::getType() const { return StateType::Close; }