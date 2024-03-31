#include <CentralizeCameraCommand.hpp>

#include <Camera.hpp>
#include <Object.hpp>

CentralizeCameraCommand::CentralizeCameraCommand(
    Camera &_camera,
    Object &_object)
    : camera(_camera),
      object(_object) {}

void CentralizeCameraCommand::execute()
{
  camera.setTarget(object.getCenter());
}
