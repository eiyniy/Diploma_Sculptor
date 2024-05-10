#include <EditState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <BaseState.hpp>
#include <EditInputEngine.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>

#include <memory>
#include <utility>

class Camera;
class IEvent;
class MainWindow;

EditState::EditState(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera,
    std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>> _objects)
    : BaseState(
          std::make_unique<EditInputEngine>(std::move(_eventBus)),
          std::make_unique<ModelRenderEngine>(
              std::move(_mainWindow), std::move(_camera), std::move(_objects)))
{
}

StateType EditState::getType() const { return StateType::Edit; }
