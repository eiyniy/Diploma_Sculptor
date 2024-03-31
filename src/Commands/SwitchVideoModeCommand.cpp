#include <SwitchVideoModeCommand.hpp>

class Camera;
class MainWindow;

SwitchVideoModeCommand::SwitchVideoModeCommand(
    Camera &_camera,
    MainWindow &_mainWindow,
    const bool _isEscape)
    : camera(_camera),
      mainWindow(_mainWindow),
      isEscape(_isEscape) {}

void SwitchVideoModeCommand::execute()
{
  // TODO
}
