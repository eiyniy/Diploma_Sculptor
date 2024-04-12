#pragma once

#include <Camera.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <glm/fwd.hpp>

enum class AxisName;
enum class Direction;

class Engine {
public:
    Engine(const Engine&) = default;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;
    Engine(Scene _scene, MainWindow _mainWindow, Camera _camera);

    ~Engine();

    void start();

private:
    Scene scene;
    MainWindow mainWindow;
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
