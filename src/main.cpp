#include <MainWindow.hpp>
#include <ShaderProgramm.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Engine.hpp>
#include <Settings.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// IWYU pragma: begin_keep
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// IWYU pragma: end_keep

#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    MainWindow mainWindow{Settings::get()->getStartupResolution()};

    Camera camera{
        Settings::get()->getCameraSpeed(),
        Settings::get()->getCameraFoV()};

    Scene scene{};

    Engine engine{scene, mainWindow, camera};

    glfwSwapInterval(Settings::get()->isVSyncEnabled());

    if (Settings::get()->isDepthBufferEnabled())
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    glfwSetInputMode(
        mainWindow.get(),
        GLFW_CURSOR,
        GLFW_CURSOR_DISABLED);

    // Uncommenting this call will result in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    engine.start();

    return 0;
}
