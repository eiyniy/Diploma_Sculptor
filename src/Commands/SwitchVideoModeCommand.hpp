#pragma once

#include <Command.hpp>

class Camera;
class MainWindow;

class SwitchVideoModeCommand : public Command
{
public:
    SwitchVideoModeCommand(
        Camera &_camera,
        MainWindow &_mainWindow,
        bool _isEscape);

    ~SwitchVideoModeCommand() override = default;

    void execute() override;

private:
    // TODO Do NOT store links (&) in all commands!
    Camera &camera;
    MainWindow &mainWindow;

    const bool isEscape;
};
