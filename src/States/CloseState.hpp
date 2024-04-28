#pragma once

#include <BaseState.hpp>

class CloseState : public BaseState {
public:
    CloseState(
        const std::shared_ptr<MainWindow>& _mainWindow,
        const std::shared_ptr<Camera>& _camera);

    [[nodiscard]] StateType getType() const override;
};