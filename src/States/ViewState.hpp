#pragma once

#include <BaseState.hpp>
#include <Camera.hpp>
#include <MainWindow.hpp>

class ViewState : public BaseState {
public:
    ViewState(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera,
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _projectionMat);

    [[nodiscard]] StateType getType() const override;
};