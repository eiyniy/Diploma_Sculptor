#include <Engine.hpp>

#include <BrushMoveEvent.hpp>
#include <CameraMoveEvent.hpp>
#include <CameraRotateEvent.hpp>
#include <CloseState.hpp>
#include <EditState.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>
#include <Object.hpp>
#include <Scene.hpp>
#include <Sculptor.hpp>
#include <SculptorEditEvent.hpp>
#include <Settings.hpp>
#include <ViewState.hpp>

#include <matrix_float4x4.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>

Engine::Engine(
    std::unique_ptr<Scene> _scene,
    const std::shared_ptr<MainWindow>& _mainWindow,
    const std::shared_ptr<Camera>& _camera)
    : scene(std::move(_scene))
    , mainWindow(_mainWindow)
    , camera(_camera)
    , deltaTime(0.F)
    , lastFrameTime(0.F)
    , state(nullptr)
    , eventBus(std::make_shared<std::queue<std::unique_ptr<IEvent>>>())
    , modelMat(std::make_shared<glm::mat4>(1))
    , viewMat(std::make_shared<glm::mat4>(1))
    , projectionMat(std::make_shared<glm::mat4>(1))
{
}

Engine::~Engine() { glfwTerminate(); }

void Engine::start()
{
    std::cout << "engine started" << std::endl;

    *projectionMat = glm::perspective(
        camera->cGetFOV(),
        mainWindow->getAspect(),
        Settings::get()->getZNear(),
        Settings::get()->getZFar());

    state = std::make_unique<ViewState>(
        eventBus, mainWindow, camera, modelMat, projectionMat);
    state->setWindowUserPointer(mainWindow);

    while (state->getType() != StateType::Close) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        updateEvents();
        updateState();

        draw();
    }
}

void Engine::updateEvents()
{
    while (!eventBus->empty()) {
        const auto event = std::move(eventBus->front());
        eventBus->pop();

        // std::cout << "event" << std::endl;

        switch (event->getType()) {
        case EventType::CameraMove: {
            const auto* const cameraMoveEvent
                = dynamic_cast<CameraMoveEvent*>(event.get());
            camera->move(
                cameraMoveEvent->getAxisName(),
                cameraMoveEvent->getDirection(),
                deltaTime);
            break;
        }
        case EventType::CameraRotate: {
            const auto* const cameraRotateEvent
                = dynamic_cast<CameraRotateEvent*>(event.get());
            camera->rotate(cameraRotateEvent->getCoordOffset(), deltaTime);
            break;
        }
        case EventType::BrushMove: {
            const auto* const brushMoveEvent
                = dynamic_cast<BrushMoveEvent*>(event.get());
            break;
        }
        case EventType::SculptorEdit: {
            const auto* const sculptorEditEvent
                = dynamic_cast<SculptorEditEvent*>(event.get());

            glm::vec3 rayOrig;
            glm::vec3 rayDir;
            Sculptor::getRayWorld(
                sculptorEditEvent->getMousePos(),
                camera->cGetPos(),
                mainWindow->getActiveResolution(),
                *projectionMat,
                camera->cGetViewMat(),
                rayOrig,
                rayDir);

            const auto object = scene->getObject("OBJECT");

            const auto verticesId = Sculptor::getSelectedTriangleVerticesIds(
                object->getTrVertices(), object->getIndices(), rayOrig, rayDir);

            if (verticesId.has_value()) {
                const auto transform = Sculptor::getTransform(
                    { verticesId->begin(), verticesId->end() });

                object->bindVerticesVBO();
                object->performTransform(transform);
                object->unbindVBO();
            }
            break;
        }
        case EventType::Close:
            mainWindow->close();
            break;
        case EventType::CaptureMouse:
            mainWindow->captureMouse();
            break;
        case EventType::ReleaseMouse:
            mainWindow->releaseMouse();
            break;
        }
    }
}

void Engine::updateState()
{
    const auto res = state->update(deltaTime);
    if (!res.has_value()) {
        return;
    }

    switch (*res) {
    case StateType::View:
        state = std::make_unique<ViewState>(
            eventBus, mainWindow, camera, modelMat, projectionMat);
        break;
    case StateType::Edit:
        state = std::make_unique<EditState>(
            eventBus, mainWindow, camera, modelMat, projectionMat);
        break;
    case StateType::Close:
        state = std::make_unique<CloseState>(
            eventBus, mainWindow, camera, modelMat, projectionMat);
        break;
    }

    state->setWindowUserPointer(mainWindow);
}

void Engine::draw()
{
    auto& objects = scene->getAllObjects();

    state->draw(objects);
}

void Engine::addObject(const std::string& name, std::unique_ptr<Object> object)
{
    scene->addObject(name, std::move(object));
}
