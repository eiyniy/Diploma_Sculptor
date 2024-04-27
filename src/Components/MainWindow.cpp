#include "BaseInputEngine.hpp"
#include <MainWindow.hpp>

#include <Settings.hpp>

#include <type_vec4.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>

MainWindow::MainWindow(const std::pair<int, int> _resolution)
    : resolution(_resolution)
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(
        GLFW_RESIZABLE, static_cast<int>(Settings::get()->isResizeEnabled()));

    window = glfwCreateWindow(
        resolution.first,
        resolution.second,
        Settings::get()->getWindowName().c_str(),
        nullptr,
        nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glewExperimental = GL_TRUE;
    glewInit();

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    activeResolution = { width, height };
}

void MainWindow::clear()
{
    const auto clearColor = Settings::get()->getWindowClearColor();

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    if (Settings::get()->isDepthBufferEnabled()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void MainWindow::swapBuffers() { glfwSwapBuffers(window); }

void MainWindow::close() { glfwSetWindowShouldClose(window, GL_TRUE); }

void MainWindow::captureMouse()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MainWindow::releaseMouse()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MainWindow::keyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mode)
{
    auto* obj = glfwGetWindowUserPointer(window);
    auto* inputEngine = static_cast<BaseInputEngine*>(obj);

    inputEngine->keyCallbackInner(key, scancode, action, mode);
}

void MainWindow::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* obj = glfwGetWindowUserPointer(window);
    auto* inputEngine = static_cast<BaseInputEngine*>(obj);

    inputEngine->mouseCallbackInner(xpos, ypos);
}

std::pair<int, int> MainWindow::getActiveResolution() const
{
    return activeResolution;
}

bool MainWindow::shouldClose() const
{
    return glfwWindowShouldClose(window) != 0;
}

GLfloat MainWindow::getAspect() const
{
    return static_cast<GLfloat>(activeResolution.first)
        / static_cast<GLfloat>(activeResolution.second);
}

GLFWwindow* MainWindow::get() const { return window; }
