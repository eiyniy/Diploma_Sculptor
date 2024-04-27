#pragma once

#include <ModelInputEngine.hpp>

#include <memory>

class MainWindow;
class Camera;

class EditInputEngine : public ModelInputEngine {
public:
    EditInputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    std::unique_ptr<BaseState> update(float dt) override;
};