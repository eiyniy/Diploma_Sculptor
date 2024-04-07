#pragma once

#include <GL/glew.h>

// IWYU pragma: begin_keep
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// IWYU pragma: end_keep

#include <utility>

enum class AxisName;
enum class Direction;

class Camera
{
private:
    GLfloat yaw, pitch;
    GLfloat mouseSens;

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 worldUp;

    glm::mat4 view;

    GLfloat speed;

    const GLfloat fov;

    void updateFront(
        const GLfloat pitch,
        const GLfloat yaw);

public:
    Camera(
        const GLfloat _speed,
        const GLfloat _fov);

    void updateViewMat();

    void move(
        const AxisName axis,
        const Direction direction,
        const GLfloat dt);

    void rotate(const std::pair<GLfloat, GLfloat> coordOffset);

    const glm::mat4 &cGetViewMat() const;

    GLfloat cGetFOV() const;
};

inline GLfloat Camera::cGetFOV() const
{
    return fov;
}

inline const glm::mat4 &Camera::cGetViewMat() const
{
    return view;
}
