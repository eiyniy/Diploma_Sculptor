#pragma once

#include <Command.hpp>

#include <queue>
#include <memory>

class MainWindow;
class Scene;
class Sculptor;

enum class AxisName;
enum class Direction;

class Engine
{
public:
    Engine(Scene &_scene, MainWindow &_mainWindow, Sculptor &_sculptor);

    void start();

private:
    const int defaultFps = 165;

    Scene &scene;
    MainWindow &mainWindow;
    Sculptor &sculptor;

    AxisName moveAxis;
    Direction moveDirection;

    // sf::Clock clock;
    int dt;

    std::queue<std::unique_ptr<Command>> commandsQueue;

    void handleEvents();

    // TODO
    // void updateInput(const sf::Event &event);

    // TODO
    // void sendInputCommand(const sf::Event &event);

    void update();

    void draw();
};