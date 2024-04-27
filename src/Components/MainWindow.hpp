#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstddef>
#include <utility>

class MainWindow {
public:
    static constexpr std::size_t keysLength = 1024;

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

    [[nodiscard]] const std::array<bool, keysLength>& cGetKeys() const;

    GLFWwindow* get();

    std::pair<GLfloat, GLfloat> resetCoordOffset();

    void captureMouse();
    void releaseMouse();

    void clear();

    void swapBuffers();

    void close();
};
