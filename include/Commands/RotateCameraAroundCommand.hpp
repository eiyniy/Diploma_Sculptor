#pragma once

#include <Command.hpp>

class Camera;
enum class AxisName;
enum class Direction;

class RotateCameraAroundCommand : public Command
{
public:
    RotateCameraAroundCommand(
        Camera &_camera,
        AxisName _axisName,
        Direction _direction,
        double _step);

    ~RotateCameraAroundCommand() override = default;

    void execute() override;

protected:
    Camera &camera;

    const AxisName axisName;
    const Direction direction;
    const double step;
};
