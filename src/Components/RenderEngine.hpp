#pragma once

#include <matrix_float4x4.hpp>
#include <qualifier.hpp>

#include <map>
#include <memory>
#include <string>

class Camera;
class MainWindow;
class Object;

class RenderEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projectionMat;

public:
    RenderEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    void draw(const std::map<std::string, std::shared_ptr<Object>>& objects);

    [[nodiscard]] bool shouldClose() const;
};