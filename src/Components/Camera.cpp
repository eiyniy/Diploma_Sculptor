#include <Camera.hpp>

#include <Enums.hpp>
#include <Settings.hpp>

#include <geometric.hpp>
#include <matrix_float4x4.hpp>
#include <matrix_transform.hpp>
#include <qualifier.hpp>
#include <trigonometric.hpp>
#include <type_mat4x4.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>

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

void Camera::rotate(const std::pair<float, float> coordOffset, const float dt)
{
    yaw += coordOffset.first * mouseSens * dt;
    pitch += coordOffset.second * mouseSens * dt;

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

GLfloat Camera::cGetFOV() const { return fov; }

const glm::mat4& Camera::cGetViewMat() const { return view; }

const glm::vec3& Camera::cGetPos() const { return pos; }
