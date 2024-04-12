#include <Camera.hpp>

#include <Enums.hpp>
#include <Settings.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

Camera::Camera(const GLfloat _speed, const GLfloat _fov)
    : yaw(startYaw)
    , pitch(startPitch)
    , mouseSens(Settings::get()->getCameraMouseSens())
    , pos(startPos)
    , front(startFront)
    , worldUp(startWorldUp)
    , view(1)
    , speed(_speed)
    , fov(_fov)
{
    updateViewMat();
}

void Camera::updateViewMat() { view = glm::lookAt(pos, pos + front, worldUp); }

void Camera::move(
    const AxisName axis, const Direction direction, const GLfloat dt)
{
    glm::vec3 step { 0 };

    switch (axis) {
    case AxisName::X:
        step = glm::normalize(glm::cross(front, worldUp)) * speed * dt;
        break;
    case AxisName::Z:
        step = front * speed * dt;
        break;
    case AxisName::Y:
        step = worldUp * speed * dt;
        break;
    }

    pos += direction == Direction::Forward ? step : -step;

    updateViewMat();
}

void Camera::rotate(const std::pair<GLfloat, GLfloat> coordOffset)
{
    yaw += coordOffset.first * mouseSens;
    pitch += coordOffset.second * mouseSens;

    if (pitch > maxPitch) {
        pitch = maxPitch;
    }
    if (pitch < minPitch) {
        pitch = minPitch;
    }

    updateFront();
}

void Camera::updateFront()
{
    front = glm::normalize(
        glm::vec3 { cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                    sin(glm::radians(pitch)),
                    cos(glm::radians(pitch)) * sin(glm::radians(yaw)) });

    updateViewMat();
}

/*
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
*/
