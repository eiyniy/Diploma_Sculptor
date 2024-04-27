#pragma once

#include <Camera.hpp>

#include <array>
#include <memory>

class BaseState;
class MainWindow;

static constexpr std::size_t keysLength = 1024;

// TODO: Add commands queue
class BaseInputEngine {
private:
    // TODO: Mb unique
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<Camera> camera;

    std::array<bool, keysLength> keys;

public:
    BaseInputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    BaseInputEngine(const BaseInputEngine&) = delete;
    BaseInputEngine(BaseInputEngine&&) = delete;
    BaseInputEngine& operator=(const BaseInputEngine&) = delete;
    BaseInputEngine& operator=(BaseInputEngine&&) = delete;

    virtual ~BaseInputEngine() = default;

    virtual std::unique_ptr<BaseState> update(float dt) = 0;

    virtual void keyCallbackInner(int key, int scancode, int action, int mode);

    virtual void mouseCallbackInner(double xpos, double ypos) = 0;

    [[nodiscard]] std::shared_ptr<MainWindow> getMainWindow() const;
    [[nodiscard]] std::shared_ptr<Camera> getCamera() const;

    [[nodiscard]] const std::array<bool, keysLength>& cGetKeys() const;
};