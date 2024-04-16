#pragma once

#include <mat4x4.hpp>
#include <qualifier.hpp>

#include <GL/glew.h>

#include <memory>

class Scene;
class MainWindow;
class Camera;

enum class AxisName;
enum class Direction;

class Engine {
public:
    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    Engine(
        std::unique_ptr<Scene> _scene,
        std::unique_ptr<MainWindow> _mainWindow,
        std::unique_ptr<Camera> _camera);

    ~Engine();

    void start();

private:
    std::unique_ptr<Scene> scene;
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<Camera> camera;

    AxisName moveAxis;
    Direction moveDirection;

    GLfloat deltaTime;
    GLfloat lastFrameTime;

    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projectionMat;

    void update();

    void draw();
};
