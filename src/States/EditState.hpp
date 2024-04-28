#pragma once

#include <BaseState.hpp>
#include <Camera.hpp>
#include <MainWindow.hpp>

class EditState : public BaseState {
public:
    EditState(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    [[nodiscard]] StateType getType() const override;
};