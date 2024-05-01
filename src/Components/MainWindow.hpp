#pragma once

#include <Concepts.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>

class MainWindow {
private:
    GLFWwindow* window;

    bool isContextSet;

    std::pair<int, int> resolution;
    std::pair<int, int> activeResolution;

    static void keyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mode);

    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

    static void
    mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

public:
    MainWindow(std::pair<int, int> _resolution);

    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
    MainWindow(const MainWindow&) = delete;

    ~MainWindow() = default;

    [[nodiscard]] std::pair<int, int> getActiveResolution() const;

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] GLfloat getAspect() const;

    [[nodiscard]] GLFWwindow* get() const;

    void captureMouse();
    void releaseMouse();

    template <class T>
        requires IsInputEnginePt<T>
    void setUserPointer(T inputEngine);

    void clear() const;

    void swapBuffers();

    void close();
};

template <class T>
    requires IsInputEnginePt<T>
void MainWindow::setUserPointer(T inputEngine)
{
    glfwSetWindowUserPointer(window, inputEngine);
}
