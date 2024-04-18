#pragma once

#include <InputEngine.hpp>
#include <RenderEngine.hpp>

#include <GL/glew.h> // IWYU pragma: keep

#include <memory>

class Scene;
class MainWindow;
class Camera;

enum class AxisName;
enum class Direction;

class Engine {
private:
    std::unique_ptr<Scene> scene;
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

    RenderEngine renderEngine;
    InputEngine inputEngine;

    AxisName moveAxis;
    Direction moveDirection;

    GLfloat deltaTime;
    GLfloat lastFrameTime;

    void update();

    void draw();

public:
    Engine(
        std::unique_ptr<Scene> _scene,
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    ~Engine();

    void start();
};
