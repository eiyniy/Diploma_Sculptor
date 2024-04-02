#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <Point.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MainWindow
{
public:
    MainWindow(const Point &_resolution);

    void setKeyCallback(GLFWkeyfun callback);

    Point getActiveResolution() const;

    bool shouldClose() const;

    void clear();

    void swapBuffers();

private:
    GLFWwindow *window;

    Point resolution;
    Point activeResolution;
};

inline Point MainWindow::getActiveResolution() const
{
    return activeResolution;
}

inline bool MainWindow::shouldClose() const
{
    return glfwWindowShouldClose(window);
}
