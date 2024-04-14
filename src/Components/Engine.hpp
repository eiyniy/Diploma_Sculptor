#pragma once

#include <Camera.hpp>
#include <Scene.hpp>

#include <matrix_float4x4.hpp>
#include <qualifier.hpp>

#include <GL/glew.h>

#include <memory>

class MainWindow;

enum class AxisName;
enum class Direction;

class Engine {
public:
    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    Engine(
        Scene _scene, std::unique_ptr<MainWindow> _mainWindow, Camera _camera);

    ~Engine();

    void start();

private:
    Scene scene;
    std::unique_ptr<MainWindow> mainWindow;
    Camera camera;

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
