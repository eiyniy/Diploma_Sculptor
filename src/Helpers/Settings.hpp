#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <GL/glew.h>

#include <mutex>
#include <string>

class Settings {
private:
    static Settings* instance;
    static std::mutex mutex;

    GLfloat cameraSpeed;
    GLfloat cameraFoV;

    GLfloat cameraMouseSens;

    float zNear;
    float zFar;

    glm::vec<4, GLclampf> windowClearColor;

    bool vSyncEnabled;
    bool depthBufferEnabled;
    bool resizeEnabled;

    std::string windowName;

protected:
    Settings();
    ~Settings() = default;

public:
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;
    Settings(Settings& other) = delete;
    void operator=(const Settings&) = delete;

    static Settings* get();

    [[nodiscard]] GLfloat getCameraSpeed() const;
    [[nodiscard]] GLfloat getCameraFoV() const;
    [[nodiscard]] GLfloat getCameraMouseSens() const;

    [[nodiscard]] float getZNear() const;
    [[nodiscard]] float getZFar() const;

    [[nodiscard]] glm::vec<4, GLclampf> getWindowClearColor() const;

    [[nodiscard]] bool isVSyncEnabled() const;
    [[nodiscard]] bool isDepthBufferEnabled() const;
    [[nodiscard]] bool isResizeEnabled() const;

    const std::string& getWindowName();
};

inline GLfloat Settings::getCameraSpeed() const { return cameraSpeed; }

inline GLfloat Settings::getCameraFoV() const { return cameraFoV; }

inline GLfloat Settings::getCameraMouseSens() const { return cameraMouseSens; }

inline float Settings::getZNear() const { return zNear; }

inline float Settings::getZFar() const { return zFar; }

inline glm::vec<4, GLclampf> Settings::getWindowClearColor() const
{
    return windowClearColor;
}

inline bool Settings::isVSyncEnabled() const { return vSyncEnabled; }

inline bool Settings::isDepthBufferEnabled() const
{
    return depthBufferEnabled;
}

inline bool Settings::isResizeEnabled() const { return resizeEnabled; }

inline const std::string& Settings::getWindowName() { return windowName; }
