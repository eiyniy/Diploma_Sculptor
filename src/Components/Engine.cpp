#include "ViewState.hpp"
#include <Engine.hpp>

#include <Enums.hpp>
#include <Object.hpp>
#include <Scene.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
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
    , state(std::make_unique<ViewState>(_mainWindow, _camera))
{
}

Engine::~Engine() { glfwTerminate(); }

void Engine::start()
{
    std::cout << "engine started" << std::endl;

    while (state->getType() != StateType::Close) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        update();

        draw();
    }
}

void Engine::update()
{
    auto res = state->update(deltaTime);
    if (res != nullptr) {
        state = std::move(res);
    }
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
