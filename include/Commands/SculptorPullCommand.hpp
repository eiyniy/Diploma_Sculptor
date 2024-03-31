#pragma once

#include <Command.hpp>
#include <Point.hpp>
#include <MainWindow.hpp>
#include <Matrix.hpp>

class Object;
class Sculptor;

class SculptorPullCommand : public Command
{
public:
    SculptorPullCommand(
        Sculptor &_sculptor,
        Object *_object,
        const MainWindow &_mainWindow,
        const Point &_mousePos,
        const Vector<4> &_direction);

    ~SculptorPullCommand() override = default;

    void execute() override;

protected:
    Sculptor &sculptor;
    Object *object;
    const MainWindow &mainWindow;
    const Point mousePos;
    const Vector<4> direction;
};
