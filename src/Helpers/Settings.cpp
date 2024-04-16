#include <Settings.hpp>

#include <trigonometric.hpp>
#include <type_vec4.hpp>

Settings* Settings::instance { nullptr };
std::mutex Settings::mutex {};

constexpr float defaultCameraSpeed = 3.F;
constexpr float defaultCameraFoV = glm::radians(45.F);
constexpr float defaultCameraMouseSens = 0.05F;
constexpr float defaultZNear = 0.1F;
constexpr float defaultZFar = 100.F;
constexpr glm::vec<4, GLclampf> defaultWindowClearColor {
    0.2F, 0.3F, 0.3F, 1.0F
};

Settings::Settings()
    : cameraSpeed(defaultCameraSpeed)
    , cameraFoV(defaultCameraFoV)
    , cameraMouseSens(defaultCameraMouseSens)
    , vSyncEnabled(false)
    , depthBufferEnabled(true)
    , resizeEnabled(false)
    , windowName("Sculptor")
    , zNear(defaultZNear)
    , zFar(defaultZFar)
    , windowClearColor(defaultWindowClearColor)
{
}

Settings* Settings::get()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new Settings();
    }

    return instance;
}
