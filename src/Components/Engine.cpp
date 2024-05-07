#include <Engine.hpp>

#include <BaseState.hpp>
#include <Brush.hpp>
#include <BrushMoveEvent.hpp>
#include <Camera.hpp>
#include <CameraMoveEvent.hpp>
#include <CameraRotateEvent.hpp>
#include <CloseState.hpp>
#include <EditState.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>
#include <LoadingState.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <ObjectsLoadedEvent.hpp>
#include <Scene.hpp>
#include <Sculptor.hpp>
#include <SculptorEditEvent.hpp>
#include <Settings.hpp>
#include <ShaderProgramManager.hpp>
#include <ViewState.hpp>

#include <matrix_clip_space.hpp>
#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

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
    , sculptor({ 1, 0.02F })
{
    processEventMap[EventType::CameraMove] = &Engine::processCameraMoveEvent;
    processEventMap[EventType::CameraRotate]
        = &Engine::processCameraRotateEvent;
    processEventMap[EventType::BrushMove] = &Engine::processBrushMoveEvent;
    processEventMap[EventType::SculptorEdit]
        = &Engine::processSculptorEditEvent;
    processEventMap[EventType::Close] = &Engine::processCloseEvent;
    processEventMap[EventType::CaptureMouse]
        = &Engine::processCaptureMouseEvent;
    processEventMap[EventType::ReleaseMouse]
        = &Engine::processReleaseMouseEvent;
    processEventMap[EventType::ObjectsLoaded]
        = &Engine::processObjectsLoadedEvent;
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

    auto shaderProgramManager = std::make_shared<ShaderProgramManager>(
        R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders)");

    shaderProgramManager->loadModelShaderProgram(
        modelMat, viewMat, projectionMat, camera);
    shaderProgramManager->loadLinesShaderProgram(
        modelMat, viewMat, projectionMat);

    state = std::make_unique<LoadingState>(
        eventBus, mainWindow, shaderProgramManager);
    state->setWindowUserPointer(mainWindow);

    while (state->getType() != StateType::Close) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        *viewMat = camera->cGetViewMat();

        updateEvents();
        updateState();

        draw();
    }
}

void Engine::updateEvents()
{
    while (!eventBus->empty()) {
        auto event = std::move(eventBus->front());
        eventBus->pop();

        processEventMap.at(event->getType())(this, std::move(event));
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
        state = std::make_unique<ViewState>(eventBus, mainWindow, camera);
        break;
    case StateType::Edit:
        state = std::make_unique<EditState>(eventBus, mainWindow, camera);
        break;
    case StateType::Close:
        state = std::make_unique<CloseState>(eventBus, mainWindow, camera);
        break;
    case StateType::Loading:
        // TODO: throw
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

void Engine::processCameraMoveEvent(std::unique_ptr<IEvent> event)
{
    const auto* const cameraMoveEvent
        = dynamic_cast<CameraMoveEvent*>(event.get());
    camera->move(
        cameraMoveEvent->getAxisName(),
        cameraMoveEvent->getDirection(),
        deltaTime);
}

void Engine::processCameraRotateEvent(std::unique_ptr<IEvent> event)
{
    const auto* const cameraRotateEvent
        = dynamic_cast<CameraRotateEvent*>(event.get());
    camera->rotate(cameraRotateEvent->getCoordOffset(), deltaTime);
}

void Engine::processBrushMoveEvent(std::unique_ptr<IEvent> event)
{
    const auto* const brushMoveEvent
        = dynamic_cast<BrushMoveEvent*>(event.get());
}

void Engine::processSculptorEditEvent(std::unique_ptr<IEvent> event)
{
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

    const auto object = scene->getObject("MODEL");

    auto&& intersectionVerticesId
        = object->getFirstRayIntersection(rayOrig, rayDir);

    if (!intersectionVerticesId.has_value()) {
        return;
    }

    std::vector<std::size_t> intersectionVerticesIdVector {
        intersectionVerticesId->begin(), intersectionVerticesId->end()
    };

    const auto transform = sculptor.getTransform(
        std::move(intersectionVerticesIdVector),
        object->getFaceNormalAverage(*intersectionVerticesId),
        sculptorEditEvent->isInverted());

    object->bindVerticesVBO();
    object->performTransform(transform);
    object->unbindVBO();
}

void Engine::processCloseEvent(std::unique_ptr<IEvent> event)
{
    mainWindow->close();
}

void Engine::processCaptureMouseEvent(std::unique_ptr<IEvent> event)
{
    mainWindow->captureMouse();
}

void Engine::processReleaseMouseEvent(std::unique_ptr<IEvent> event)
{
    mainWindow->releaseMouse();
}

void Engine::processObjectsLoadedEvent(std::unique_ptr<IEvent> event)
{
    auto* const objectsLoadedEvent
        = dynamic_cast<ObjectsLoadedEvent*>(event.get());

    for (auto&& object : objectsLoadedEvent->getObjects()) {
        addObject(std::string(object->getName()), std::move(object));
    }
}
