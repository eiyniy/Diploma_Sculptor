#pragma once

#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_vec3.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>

#include <utility>

enum class AxisName;
enum class Direction;

constexpr GLfloat startYaw = -90.F;
constexpr GLfloat startPitch = 0.F;

constexpr GLfloat minPitch = -89.F;
constexpr GLfloat maxPitch = 89.F;

constexpr auto startPos = glm::vec3(0.F, 0.F, 0.F);
constexpr auto startFront = glm::vec3(0.F, 0.F, -1.F);

constexpr auto startWorldUp = glm::vec3(0.F, 1.F, 0.F);

class Camera {
private:
    GLfloat yaw, pitch;
    GLfloat mouseSens;

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 worldUp;

    glm::mat4 view;

    GLfloat speed;

    GLfloat fov;

    void updateFront();

public:
    Camera(GLfloat _speed, GLfloat _fov);

    void updateViewMat();

    void move(AxisName axis, Direction direction, GLfloat dt);

    void rotate(std::pair<float, float> coordOffset, float dt);

    [[nodiscard]] const glm::mat4& cGetViewMat() const;

    [[nodiscard]] GLfloat cGetFOV() const;

    [[nodiscard]] const glm::vec3& cGetPos() const;
};
