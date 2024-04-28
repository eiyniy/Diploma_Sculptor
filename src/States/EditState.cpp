#include <EditState.hpp>

#include <BaseState.hpp>
#include <EditInputEngine.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <memory>

EditState::EditState(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : BaseState(
          std::make_unique<EditInputEngine>(_mainWindow, _camera),
          std::make_unique<ModelRenderEngine>(_mainWindow, _camera))
{
}

StateType EditState::getType() const { return StateType::Edit; }
