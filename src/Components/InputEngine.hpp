#pragma once

#include <Gl/glew.h>

#include <memory>

class MainWindow;
class Camera;

class InputEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

public:
    InputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    void update(GLfloat dt);
};