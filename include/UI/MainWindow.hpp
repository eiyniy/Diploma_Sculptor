#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <Point.hpp>

class MainWindow
{
public:
    MainWindow(Point &_resolution);

    void clear();

private:
    GLFWwindow *window;

    Point &resolution;
};
