#pragma once

#include <Command.hpp>
#include <MainWindow.hpp>
#include <Matrix.hpp>

#include <utility>

class Object;
class Sculptor;

class SculptorPullCommand : public Command
{
public:
    SculptorPullCommand(
        Sculptor &_sculptor,
        Object *_object,
        const MainWindow &_mainWindow,
        const std::pair<int, int> &_mousePos,
        const Vector<4> &_direction);

    ~SculptorPullCommand() override = default;

    void execute() override;

protected:
    Sculptor &sculptor;
    Object *object;
    const MainWindow &mainWindow;
    const std::pair<int, int> mousePos;
    const Vector<4> direction;
};
