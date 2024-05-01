#pragma once

#include <BaseRenderEngine.hpp>

#include <matrix_float4x4.hpp>

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

    std::shared_ptr<glm::mat4> modelMat;
    std::shared_ptr<glm::mat4> projectionMat;

public:
    ModelRenderEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera,
        std::shared_ptr<glm::mat4> _modelMat,
        std::shared_ptr<glm::mat4> _projectionMat);

    void draw(
        const std::map<std::string, std::shared_ptr<Object>>& objects) override;

    [[nodiscard]] bool shouldClose() const;
};