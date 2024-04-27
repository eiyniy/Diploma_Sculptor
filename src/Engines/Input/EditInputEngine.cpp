#include <EditInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>
#include <ViewState.hpp>

#include <GLFW/glfw3.h>

#include <utility>

EditInputEngine::EditInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : ModelInputEngine(std::move(_mainWindow), std::move(_camera))
{
    getMainWindow()->releaseMouse();
}

std::unique_ptr<BaseState> EditInputEngine::update(const float dt)
{
    auto res = ModelInputEngine::update(dt);
    if (res != nullptr) {
        return std::move(res);
    }

    const auto& keys = cGetKeys();

    if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_M]
        && isKeysDelayElapsed()) {
        return std::make_unique<ViewState>(getMainWindow(), getCamera());
    }

    return nullptr;
}