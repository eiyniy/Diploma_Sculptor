#include <SculptorPullCommand.hpp>

#include <Sculptor.hpp>
#include <Object.hpp>
#include <Matrix.hpp>

class MainWindow;
class Point;

SculptorPullCommand::SculptorPullCommand(
    Sculptor &_sculptor,
    Object *_object,
    const MainWindow &_mainWindow,
    const Point &_mousePos,
    const Vector<4> &_direction)
    : sculptor(_sculptor),
      object(_object),
      mainWindow(_mainWindow),
      mousePos(_mousePos),
      direction(_direction) {}

void SculptorPullCommand::execute()
{
    // std::cout << "SculptorPullCommand log:" << std::endl;
    // direction.log();

    // TODO: Get pointed ID by Ray Casting from camera to mouse and to 3d object

    sculptor.createGraph(object);
    sculptor.pull(
        object->getVertices(),
        mousePos,
        direction);

    /*
    const auto pointedId = mainWindow.getPointedTriangleId(object, mousePos);
    sculptor.pull(object, pointedId, mousePos);
    */
}