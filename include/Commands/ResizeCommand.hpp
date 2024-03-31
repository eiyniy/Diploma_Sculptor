#pragma once

#include <Command.hpp>

class Camera;
class MainWindow;

class ResizeCommand : public Command
{
public:
    ResizeCommand(
        Camera &_camera,
        MainWindow &_mainWindow,
        int _width,
        int _height);

    ~ResizeCommand() override = default;

    void execute() override;

private:
    Camera &camera;
    MainWindow &mainWindow;

    const int width;
    const int height;
};
