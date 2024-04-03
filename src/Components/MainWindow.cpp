#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <MainWindow.hpp>

#include <Point.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

MainWindow::MainWindow(const Point &_resolution)
    : resolution(_resolution)
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(
        resolution.cGetX(),
        resolution.cGetY(),
        "Sculptor",
        nullptr,
        nullptr);
    glfwMakeContextCurrent(window);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    activeResolution = {width, height};
}

void MainWindow::setKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(window, callback);
}

void MainWindow::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWindow::swapBuffers()
{
    glfwSwapBuffers(window);
}
