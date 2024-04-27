#pragma once

#include <BaseState.hpp>
#include <Camera.hpp>
#include <MainWindow.hpp>

class ViewState : public BaseState {
public:
    ViewState(
        const std::shared_ptr<MainWindow>& _mainWindow,
        const std::shared_ptr<Camera>& _camera);

    [[nodiscard]] StateTypes getType() const override;
};