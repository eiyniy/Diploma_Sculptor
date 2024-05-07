#pragma once

#include <Enums.hpp>
#include <Sculptor.hpp>

#include <functional>
#include <matrix_float4x4.hpp>

#include <GL/glew.h>

#include <map>
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

    Sculptor sculptor;

    GLfloat deltaTime;
    GLfloat lastFrameTime;

    std::map<
        EventType,
        std::function<void(Engine*, std::unique_ptr<IEvent> event)>>
        processEventMap;

    void updateEvents();

    void updateState();

    void draw();

    void processCameraMoveEvent(std::unique_ptr<IEvent> event);
    void processCameraRotateEvent(std::unique_ptr<IEvent> event);
    void processBrushMoveEvent(std::unique_ptr<IEvent> event);
    void processSculptorEditEvent(std::unique_ptr<IEvent> event);
    void processCloseEvent(std::unique_ptr<IEvent> event);
    void processCaptureMouseEvent(std::unique_ptr<IEvent> event);
    void processReleaseMouseEvent(std::unique_ptr<IEvent> event);
    void processObjectsLoadedEvent(std::unique_ptr<IEvent> event);

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
