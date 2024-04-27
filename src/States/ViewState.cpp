#include <ViewState.hpp>

#include <BaseState.hpp>
#include <Enums.hpp>
#include <ModelRenderEngine.hpp>
#include <ViewInputEngine.hpp>

#include <memory>

ViewState::ViewState(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : BaseState(
          std::make_unique<ViewInputEngine>(_mainWindow, _camera),
          std::make_unique<ModelRenderEngine>(_mainWindow, _camera))
{
}

StateTypes ViewState::getType() const { return StateTypes::View; }
