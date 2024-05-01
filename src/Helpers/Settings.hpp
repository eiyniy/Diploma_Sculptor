#pragma once

#include <type_vec4.hpp>

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

    float shaderProgramDimmingFactor;

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

    [[nodiscard]] float getShaderProgramDimmingFactor() const;

    const std::string& getWindowName();
};
