#include <Settings.hpp>

#include <trigonometric.hpp>
#include <type_vec4.hpp>

Settings* Settings::instance { nullptr };
std::mutex Settings::mutex {};

constexpr float defaultCameraSpeed = 10.F;
constexpr float defaultCameraFoV = glm::radians(45.F);
constexpr float defaultCameraMouseSens = 0.05F;
constexpr float defaultZNear = 0.1F;
constexpr float defaultZFar = 1000.F;
constexpr glm::vec<4, GLclampf> defaultWindowClearColor {
    0.2F, 0.3F, 0.3F, 1.0F
};

Settings::Settings()
    : cameraSpeed(defaultCameraSpeed)
    , cameraFoV(defaultCameraFoV)
    , cameraMouseSens(defaultCameraMouseSens)
    , vSyncEnabled(true)
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

GLfloat Settings::getCameraSpeed() const { return cameraSpeed; }

GLfloat Settings::getCameraFoV() const { return cameraFoV; }

GLfloat Settings::getCameraMouseSens() const { return cameraMouseSens; }

float Settings::getZNear() const { return zNear; }

float Settings::getZFar() const { return zFar; }

glm::vec<4, GLclampf> Settings::getWindowClearColor() const
{
    return windowClearColor;
}

bool Settings::isVSyncEnabled() const { return vSyncEnabled; }

bool Settings::isDepthBufferEnabled() const { return depthBufferEnabled; }

bool Settings::isResizeEnabled() const { return resizeEnabled; }

const std::string& Settings::getWindowName() { return windowName; }
