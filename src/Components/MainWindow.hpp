#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstddef>
#include <utility>

constexpr std::size_t keysLength = 1024;

class MainWindow {
public:
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
    MainWindow(const MainWindow&) = delete;

    MainWindow(std::pair<int, int> _resolution);
    ~MainWindow() = default;

    [[nodiscard]] std::pair<int, int> getActiveResolution() const;

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] GLfloat getAspect() const;

    [[nodiscard]] const std::array<bool, keysLength>& cGetKeys() const;

    GLFWwindow* get();

    std::pair<GLfloat, GLfloat> resetCoordOffset();

    static void clear();

    void swapBuffers();

    void close();

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
};
