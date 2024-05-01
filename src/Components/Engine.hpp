#pragma once

#include <matrix_float4x4.hpp>

#include <GL/glew.h>

#include <memory>
#include <queue>
#include <string>

class Scene;
class MainWindow;
class Camera;

class Object;

class BaseState;
class IEvent;

class Engine {
private:
    std::unique_ptr<Scene> scene;
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

    std::unique_ptr<BaseState> state;

    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> eventBus;

    std::shared_ptr<glm::mat4> modelMat;
    std::shared_ptr<glm::mat4> viewMat;
    std::shared_ptr<glm::mat4> projectionMat;

    GLfloat deltaTime;
    GLfloat lastFrameTime;

    void updateEvents();

    void updateState();

    void draw();

public:
    Engine(
        std::unique_ptr<Scene> _scene,
        const std::shared_ptr<MainWindow>& _mainWindow,
        const std::shared_ptr<Camera>& _camera);

    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    ~Engine();

    void start();

    void addObject(const std::string& name, std::unique_ptr<Object> object);
};
