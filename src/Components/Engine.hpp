#pragma once

#include <ShaderProgramm.hpp>
#include <Texture.hpp>

// IWYU pragma: begin_keep
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// IWYU pragma: end_keep

class MainWindow;
class Scene;
class Camera;

enum class AxisName;
enum class Direction;

class Engine
{
public:
    Engine(
        Scene &_scene,
        MainWindow &_mainWindow,
        Camera &_camera);

    ~Engine();

    void start();

private:
    const int defaultFps = 165;

    Scene &scene;
    MainWindow &mainWindow;
    Camera &camera;

    AxisName moveAxis;
    Direction moveDirection;

    GLfloat deltaTime, lastFrameTime;

    GLuint VBO, VAO, EBO;
    ShaderProgram shaderProgram;

    Texture containerTexture;
    Texture faceTexture;

    glm::mat4 projectionMat;

    void
    update();

    void draw();
};
