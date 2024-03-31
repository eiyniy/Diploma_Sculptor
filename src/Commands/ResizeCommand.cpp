#include <ResizeCommand.hpp>

class Camera;
class MainWindow;

ResizeCommand::ResizeCommand(
    Camera &_camera,
    MainWindow &_mainWindow,
    const int _width,
    const int _height)
    : camera(_camera),
      mainWindow(_mainWindow),
      width(_width),
      height(_height) {}

void ResizeCommand::execute()
{
  // TODO
}
