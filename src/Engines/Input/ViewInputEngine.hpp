#pragma once

#include <ModelInputEngine.hpp>

#include <memory>

class MainWindow;
class Camera;

class ViewInputEngine : public ModelInputEngine {
private:
    std::pair<float, float> lastMousePos;
    std::pair<float, float> mousePos;

public:
    ViewInputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    std::unique_ptr<BaseState> update(float dt) override;

    void mouseCallbackInner(double xpos, double ypos) override;
};