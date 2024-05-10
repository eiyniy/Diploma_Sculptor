#pragma once

#include <BaseState.hpp>
#include <Enums.hpp>

#include <memory>
#include <queue>

class IEvent;
class MainWindow;
class ShaderProgramManager;

class LoadingState : public BaseState {
public:
    LoadingState(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
        std::shared_ptr<MainWindow> _mainWindow,
        const std::shared_ptr<ShaderProgramManager>& _shaderProgramManager);

    [[nodiscard]] StateType getType() const override;
};
