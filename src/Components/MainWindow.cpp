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
    , keys()
    , isMoved(false)
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(
        GLFW_RESIZABLE, static_cast<int>(Settings::get()->isResizeEnabled()));

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(
        resolution.first,
        resolution.second,
        Settings::get()->getWindowName().c_str(),
        nullptr,
        nullptr);
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    // Set this to true so GLEW knows to use a modern approach to retrieving
    // function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    activeResolution = { width, height };
}

std::pair<GLfloat, GLfloat> MainWindow::resetCoordOffset()
{
    if (!isMoved) {
        return { 0.F, 0.F };
    }

    isMoved = false;
    return coordOffset;
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
    double cursorPosX = NAN;
    double cursorPosY = NAN;
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    lastCoord = { cursorPosX, cursorPosY };

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
    auto* mainWindow = static_cast<MainWindow*>(obj);

    mainWindow->keyCallbackInner(key, scancode, action, mode);
}

void MainWindow::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* obj = glfwGetWindowUserPointer(window);
    auto* mainWindow = static_cast<MainWindow*>(obj);

    mainWindow->mouseCallbackInner(xpos, ypos);
}

void MainWindow::keyCallbackInner(
    const int key, const int scancode, const int action, const int mode)
{
    if (key >= keys.size()) {
        throw std::out_of_range("Key code is out of range");
    }

    if (action == GLFW_PRESS) {
        keys.at(key) = true;
    } else if (action == GLFW_RELEASE) {
        keys.at(key) = false;
    }
}

void MainWindow::mouseCallbackInner(const double xpos, const double ypos)
{
    isMoved = true;

    // Обратный порядок вычитания Y, потому что оконные Y-координаты возрастают
    // с верху вниз
    coordOffset = { xpos - lastCoord.first, lastCoord.second - ypos };

    lastCoord = { xpos, ypos };
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

const std::array<bool, MainWindow::keysLength>& MainWindow::cGetKeys() const
{
    return keys;
}

GLFWwindow* MainWindow::get() { return window; }
