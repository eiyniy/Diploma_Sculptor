#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>
#include <array>

class MainWindow
{
public:
    MainWindow(const std::pair<int, int> &_resolution);

    std::pair<int, int> getActiveResolution() const;

    bool shouldClose() const;

    GLfloat getAspect() const;

    const std::array<bool, 1024> &cGetKeys() const;

    GLFWwindow *get();

    const std::pair<GLfloat, GLfloat> resetCoordOffset();

    void clear();

    void swapBuffers();

    void close();

private:
    GLFWwindow *window;

    std::pair<int, int>
        resolution,
        activeResolution;

    std::array<bool, 1024> keys;

    std::pair<GLfloat, GLfloat>
        lastCoord,
        coordOffset;

    bool isMoved;

    static void keyCallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mode);

    static void mouseCallback(
        GLFWwindow *window,
        double xpos,
        double ypos);

    void keyCallbackInner(
        const int key,
        const int scancode,
        const int action,
        const int mode);

    void mouseCallbackInner(
        const double xpos,
        const double ypos);
};

inline std::pair<int, int> MainWindow::getActiveResolution() const
{
    return activeResolution;
}

inline bool MainWindow::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

inline GLfloat MainWindow::getAspect() const
{
    return (GLfloat)activeResolution.first / activeResolution.second;
}

inline const std::array<bool, 1024> &MainWindow::cGetKeys() const
{
    return keys;
}

inline GLFWwindow *MainWindow::get()
{
    return window;
}
