#pragma once

#include <MoveCommand.hpp>

class Object;

enum class AxisName;
enum class Direction;

class MoveObjectCommand : public MoveCommand
{
public:
    MoveObjectCommand(
        Object &_object,
        AxisName _axisName,
        Direction _direction,
        double _step);

    ~MoveObjectCommand() override = default;

    void execute() override;

private:
    Object &object;
};
