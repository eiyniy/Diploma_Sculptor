#include <Camera.hpp>

Camera::Camera(
    const CoordinateVector &p_position,
    const CoordinateVector &p_target,
    const Dot p_resolution,
    const int p_fov)
    : position(p_position),
      target(p_target),
      resolution(p_resolution),
      fov(p_fov)
{
}

const CoordinateVector &Camera::getPosition() const
{
    return position;
}

const CoordinateVector &Camera::getTarget() const
{
    return target;
}

const Dot Camera::getResolution() const
{
    return resolution;
}

const int Camera::getFOV() const
{
    return fov;
}

const double Camera::getAspect() const
{
    return ((double)resolution.x) / resolution.y;
}
