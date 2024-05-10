#pragma once

#include <BaseState.hpp>
#include <Enums.hpp>

#include <memory>
#include <queue>

class Camera;
class IEvent;
class MainWindow;

class ViewState : public BaseState {
public:
    ViewState(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera,
        std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>>
            _objects);

    [[nodiscard]] StateType getType() const override;
};