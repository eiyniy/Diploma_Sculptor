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
    MainWindow(Point &_resolution);

    void setKeyCallback(GLFWwindow *window, GLFWkeyfun callback);

    Point getActiveResolution() const;

    void clear();

private:
    GLFWwindow *window;

    Point resolution;
    Point activeResolution;
};

inline Point MainWindow::getActiveResolution() const
{
    return activeResolution;
}
