#include <Camera.hpp>

#include <SphericalCoordinate.hpp>
#include <Math.hpp>
#include <Matrix.hpp>

#include <cmath>
#include <utility>

enum class AxisName;
enum class Direction;

Camera::Camera(
    const Vector<4> &_up,
    const Vector<4> &_position,
    const Vector<4> &_target,
    std::pair<int, int> &_resolution,
    const int _fov)
    : up(_up),
      position(_position),
      target(_target),
      resolution(_resolution),
      fov((int)(_fov * M_PI / 180)) {}

void Camera::move(const Vector<4> &transition)
{
    target += transition;
    position += transition;
}

void Camera::rotateAround(
    const AxisName axisName,
    const Direction direction,
    const double step)
{
    auto cameraRelative = position - target;
    auto spherical = Math::decartToSpherical(cameraRelative);

    bool isReversed = false;
    spherical.move(axisName, direction, step, isReversed);

    if (isReversed)
        up.getY() *= -1;

    cameraRelative = Math::sphericalToDecart(spherical);
    position = cameraRelative + target;
}

void Camera::setResolution(const std::pair<int, int> &newResolution)
{
    resolution = newResolution;
}

void Camera::setTarget(const Vector<4> &newTarget)
{
    target = newTarget;
}
