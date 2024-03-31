#include <MoveObjectCommand.hpp>

#include <Object.hpp>
#include <MoveCommand.hpp>

enum class AxisName;
enum class Direction;

MoveObjectCommand::MoveObjectCommand(
    Object &_object,
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : MoveCommand(_axisName, _direction, _step),
      object(_object) {}

void MoveObjectCommand::execute()
{
  object.move(getTransition());
}
