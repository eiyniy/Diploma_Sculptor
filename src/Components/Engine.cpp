#include <Engine.hpp>

#include <BaseState.hpp>
#include <BrushMoveEvent.hpp>
#include <Camera.hpp>
#include <CameraMoveEvent.hpp>
#include <CameraRotateEvent.hpp>
#include <CloseState.hpp>
#include <EditState.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>
#include <LinesUniformLoader.hpp>
#include <LoadingState.hpp>
#include <MainWindow.hpp>
#include <ModelUniformLoader.hpp>
#include <ObjParser.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <Scene.hpp>
#include <Sculptor.hpp>
#include <SculptorEditEvent.hpp>
#include <Settings.hpp>
#include <ShaderProgramBuilder.hpp>
#include <ViewState.hpp>

#include <algorithm>
#include <matrix_clip_space.hpp>
#include <matrix_float4x4.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_vec4.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// TODO: Move to settings OR to static const
const auto* const objectSPName = "base";
const auto* const linesSPName = "lines";

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

    state = std::make_unique<LoadingState>(eventBus, mainWindow);
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

            auto&& intersectionVerticesId
                = object->getFirstRayIntersection(rayOrig, rayDir);

            if (!intersectionVerticesId.has_value()) {
                break;
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
        case EventType::EngineLoad: {
            ShaderProgramBuilder shaderProgramBuilder {};

            shaderProgramBuilder.create();

            auto modelUniformLoader = std::make_unique<ModelUniformLoader>(
                modelMat, viewMat, projectionMat, camera);

            shaderProgramBuilder.init(
                objectSPName, std::move(modelUniformLoader));

            shaderProgramBuilder.addShader(
                R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\base.vert)",
                GL_VERTEX_SHADER);
            shaderProgramBuilder.addShader(
                R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\flat\flat.frag)",
                GL_FRAGMENT_SHADER);
            // shaderProgramBuilder.addShader(
            //     R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\normals\normals.frag)",
            //     GL_FRAGMENT_SHADER);
            // shaderProgramBuilder.addShader(
            //     R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\normals\flatNormals.frag)",
            //     GL_FRAGMENT_SHADER);

            shaderProgramBuilder.link();

            shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
                ShaderProgram::positionAName,
                0,
                4,
                GL_FLOAT,
                sizeof(GLfloat),
                GL_FALSE));
            shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
                ShaderProgram::normalAName,
                1,
                3,
                GL_FLOAT,
                sizeof(GLfloat),
                GL_FALSE));

            // TODO: Add loading state before parsing

            shaderProgramBuilder.addNewUniform(ShaderProgram::modelUName);
            shaderProgramBuilder.addNewUniform(ShaderProgram::viewUName);
            shaderProgramBuilder.addNewUniform(ShaderProgram::projectionUName);

            shaderProgramBuilder.addNewUniform(ShaderProgram::cameraPosUName);

            shaderProgramBuilder.addNewUniform(ShaderProgram::lightColorUName);

            shaderProgramBuilder.addNewUniform(
                ShaderProgram::dimmingFactorUName);
            shaderProgramBuilder.addNewUniform(
                ShaderProgram::isDistanceDimmingUName);

            auto objectSP = shaderProgramBuilder.build();

            const std::string projectPath
                = R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\)";

            std::string path
                // = projectPath + R"(resources\baseModels\cube\cube.obj)";
                // = projectPath + R"(resources\baseModels\sphere\sphere.obj)";
                // = projectPath + R"(resources\models\woman1.obj)";
                = projectPath + R"(resources\models\car\car.obj)";
            // = projectPath + R"(resources\models\angel\angel.obj)";

            ObjParser parser { path };
            auto parseResult = parser.parse();

            ObjectBuilder objectBuilder {};

            const auto objectBuilderStart
                = std::chrono::high_resolution_clock::now();

            objectBuilder.create();
            objectBuilder.init(
                GL_TRIANGLES, std::move(parseResult.getVertices()));

            objectBuilder.addTriangles(std::move(parseResult.getTriangles()));

            objectBuilder.addNVertices(std::move(parseResult.getNVertices()));

            objectBuilder.addShaderProgram(std::move(objectSP));
            objectBuilder.selectShaderProgram(objectSPName);

            const auto transformStart
                = std::chrono::high_resolution_clock::now();

            objectBuilder.transform();

            const auto setupVAOStart
                = std::chrono::high_resolution_clock::now();

            objectBuilder.setupVAO();

            auto object = std::move(objectBuilder.build());

            shaderProgramBuilder.create();

            auto linesUniformLoader = std::make_unique<LinesUniformLoader>(
                modelMat, viewMat, projectionMat);

            shaderProgramBuilder.init(
                linesSPName, std::move(linesUniformLoader));

            shaderProgramBuilder.addShader(
                R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\lines\lines.vert)",
                GL_VERTEX_SHADER);
            shaderProgramBuilder.addShader(
                R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\lines\lines.frag)",
                GL_FRAGMENT_SHADER);

            shaderProgramBuilder.link();

            shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
                ShaderProgram::positionAName,
                0,
                4,
                GL_FLOAT,
                sizeof(GLfloat),
                GL_FALSE));

            shaderProgramBuilder.addNewUniform(ShaderProgram::modelUName);
            shaderProgramBuilder.addNewUniform(ShaderProgram::viewUName);
            shaderProgramBuilder.addNewUniform(ShaderProgram::projectionUName);

            shaderProgramBuilder.addNewUniform(ShaderProgram::colorUName);

            auto linesSP = shaderProgramBuilder.build();

            path = projectPath
                + R"(resources\models\armillarySphere\ArmillarySphere.obj)";

            parser = ObjParser { path };
            parseResult = parser.parse();

            objectBuilder.create();
            objectBuilder.init(GL_LINES, parseResult.getVertices());

            objectBuilder.addLines(parseResult.getLinesId());

            objectBuilder.addShaderProgram(std::move(linesSP));
            objectBuilder.selectShaderProgram(linesSPName);

            objectBuilder.transform();

            objectBuilder.setupVAO();

            auto armillarySphere = std::move(objectBuilder.build());

            std::cout << "Objects builded." << std::endl;
            std::cout << "Start stuff time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             transformStart - objectBuilderStart)
                      << std::endl;
            std::cout << "Transform time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             setupVAOStart - transformStart)
                      << std::endl;
            std::cout << "Setup VAO time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now()
                             - setupVAOStart)
                      << std::endl;

            addObject("OBJECT", std::move(object));
            addObject("SELECTOR", std::move(armillarySphere));
            break;
        }
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
        state = std::make_unique<ViewState>(eventBus, mainWindow, camera);
        break;
    case StateType::Edit:
        state = std::make_unique<EditState>(eventBus, mainWindow, camera);
        break;
    case StateType::Close:
        state = std::make_unique<CloseState>(eventBus, mainWindow, camera);
        break;
    case StateType::Loading:
        state = std::make_unique<LoadingState>(eventBus, mainWindow);
        break;
    }

    state->setWindowUserPointer(mainWindow);
}

void Engine::draw()
{
    *viewMat = camera->cGetViewMat();

    auto& objects = scene->getAllObjects();

    state->draw(objects);
}

void Engine::addObject(const std::string& name, std::unique_ptr<Object> object)
{
    scene->addObject(name, std::move(object));
}
