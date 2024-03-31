#pragma once

#include <Command.hpp>

class Camera;
class Object;

class CentralizeCameraCommand : public Command
{
public:
    CentralizeCameraCommand(
        Camera &_camera,
        Object &_object);

    ~CentralizeCameraCommand() override = default;

    void execute() override;

private:
    Camera &camera;
    Object &object;
};
