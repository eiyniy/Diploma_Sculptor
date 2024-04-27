#include <ModelInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <CloseState.hpp>
#include <EditState.hpp>

#include <chrono>
#include <memory>
#include <utility>

ModelInputEngine::ModelInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : BaseInputEngine(std::move(_mainWindow), std::move(_camera))
    , switchTime(std::chrono::high_resolution_clock::now())
{
}

std::unique_ptr<BaseState> ModelInputEngine::update(const float dt)
{
    auto res = BaseInputEngine::update(dt);
    if (res != nullptr) {
        return std::move(res);
    }

    if (cGetKeys()[GLFW_KEY_ESCAPE]) {
        getMainWindow()->close();
        return std::make_unique<CloseState>();
    }

    return nullptr;
}

bool ModelInputEngine::isKeysDelayElapsed() const
{
    return (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - switchTime))
               .count()
        > switchDelayMs;
}
