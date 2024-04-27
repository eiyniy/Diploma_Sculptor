#pragma once

#include <BaseInputEngine.hpp>

#include <chrono>
#include <memory>

class ModelInputEngine : public BaseInputEngine {
private:
    static constexpr int switchDelayMs = 100;
    std::chrono::time_point<std::chrono::high_resolution_clock> switchTime;

public:
    ModelInputEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<Camera> _camera);

    std::unique_ptr<BaseState> update(float dt) override;

    [[nodiscard]] bool isKeysDelayElapsed() const;
};