#include <Engine.hpp>

#include <Enums.hpp>
#include <InputEngine.hpp>
#include <Object.hpp>
#include <RenderEngine.hpp>
#include <Scene.hpp>

#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <utility>

Engine::Engine(
    std::unique_ptr<Scene> _scene,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera)
    : scene(std::move(_scene))
    , mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , moveAxis(AxisName::X)
    , moveDirection(Direction::Forward)
    , deltaTime(0.F)
    , lastFrameTime(0.F)
    , renderEngine { mainWindow, camera }
    , inputEngine(mainWindow, camera)
{
}

Engine::~Engine() { glfwTerminate(); }

void Engine::start()
{
    while (!renderEngine.shouldClose()) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        update();

        draw();
    }
}

void Engine::update() { inputEngine.update(deltaTime); }

void Engine::draw()
{
    auto& objects = scene->getAllObjects();

    renderEngine.draw(objects);
}

void Engine::addObject(const std::string& name, std::unique_ptr<Object> object)
{
    scene->addObject(name, std::move(object));
}
