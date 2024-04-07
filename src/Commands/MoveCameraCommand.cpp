#include <MoveCameraCommand.hpp>

// #include <Camera.hpp>
#include <MoveCommand.hpp>

enum class AxisName;
enum class Direction;

MoveCameraCommand::MoveCameraCommand(
    Camera &_camera,
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : MoveCommand(_axisName, _direction, _step),
      camera(_camera) {}

void MoveCameraCommand::execute()
{
  // camera.move(getTransition());
}
