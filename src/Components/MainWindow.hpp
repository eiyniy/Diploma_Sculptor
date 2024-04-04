#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>

class MainWindow
{
public:
    MainWindow(const std::pair<int, int> &_resolution);

    void setKeyCallback(GLFWkeyfun callback);

    std::pair<int, int> getActiveResolution() const;

    bool shouldClose() const;

    GLfloat getAspect() const;

    void clear();

    void swapBuffers();

private:
    GLFWwindow *window;

    std::pair<int, int> resolution;
    std::pair<int, int> activeResolution;
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
