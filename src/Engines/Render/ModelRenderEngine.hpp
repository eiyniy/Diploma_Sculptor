#pragma once

#include <BaseRenderEngine.hpp>

#include <map>
#include <memory>
#include <string>

class Camera;
class MainWindow;
class Object;

class ModelRenderEngine : public BaseRenderEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>> objects;

public:
    ModelRenderEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera,
        std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>>
            _objects);

    void draw() override;

    [[nodiscard]] bool shouldClose() const;
};