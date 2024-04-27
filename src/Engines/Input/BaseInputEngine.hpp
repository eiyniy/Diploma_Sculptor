#pragma once

#include <Camera.hpp>
#include <MainWindow.hpp>

#include <array>
#include <memory>

class BaseState;

// TODO: Add commands queue
class BaseInputEngine {
private:
    // TODO: Mb unique
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

    std::array<bool, MainWindow::keysLength> keys;

public:
    BaseInputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    BaseInputEngine(const BaseInputEngine&) = delete;
    BaseInputEngine(BaseInputEngine&&) = delete;
    BaseInputEngine& operator=(const BaseInputEngine&) = delete;
    BaseInputEngine& operator=(BaseInputEngine&&) = delete;

    virtual ~BaseInputEngine() = default;

    virtual std::unique_ptr<BaseState> update(float dt);

    [[nodiscard]] std::shared_ptr<MainWindow> getMainWindow() const;
    [[nodiscard]] std::shared_ptr<Camera> getCamera() const;

    [[nodiscard]] const std::array<bool, MainWindow::keysLength>&
    cGetKeys() const;
};