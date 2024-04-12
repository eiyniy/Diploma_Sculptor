#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstddef>
#include <utility>

constexpr std::size_t keysLength = 1024;

class MainWindow {
public:
    MainWindow(std::pair<int, int> _resolution);

    [[nodiscard]] std::pair<int, int> getActiveResolution() const;

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] GLfloat getAspect() const;

    [[nodiscard]] const std::array<bool, keysLength>& cGetKeys() const;

    GLFWwindow* get();

    std::pair<GLfloat, GLfloat> resetCoordOffset();

    static void clear();

    void swapBuffers();

    void close();

private:
    GLFWwindow* window;

    std::pair<int, int> resolution, activeResolution;

    std::array<bool, keysLength> keys;

    std::pair<GLfloat, GLfloat> lastCoord, coordOffset;

    bool isMoved;

    static void keyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mode);

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    void keyCallbackInner(int key, int scancode, int action, int mode);

    void mouseCallbackInner(double xpos, double ypos);
};

inline std::pair<int, int> MainWindow::getActiveResolution() const
{
    return activeResolution;
}

inline bool MainWindow::shouldClose() const
{
    return glfwWindowShouldClose(window) != 0;
}

inline GLfloat MainWindow::getAspect() const
{
    return static_cast<GLfloat>(activeResolution.first)
        / static_cast<GLfloat>(activeResolution.second);
}

inline const std::array<bool, keysLength>& MainWindow::cGetKeys() const
{
    return keys;
}

inline GLFWwindow* MainWindow::get() { return window; }
