#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <CloseState.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

CloseState::CloseState(
    const std::shared_ptr<MainWindow>& _mainWindow,
    const std::shared_ptr<Camera>& _camera)
    : BaseState(
          std::make_unique<ViewInputEngine>(_mainWindow, _camera),
          std::make_unique<ModelRenderEngine>(_mainWindow, _camera))
{
}

StateType CloseState::getType() const { return StateType::Close; }