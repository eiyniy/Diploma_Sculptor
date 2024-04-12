#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <iostream>

const std::pair<int, int> startupResolution { 1280, 720 };

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    MainWindow mainWindow { startupResolution };

    Camera camera { Settings::get()->getCameraSpeed(),
                    Settings::get()->getCameraFoV() };

    Scene scene {};

    Engine engine { scene, mainWindow, camera };

    glfwSwapInterval(static_cast<int>(Settings::get()->isVSyncEnabled()));

    if (Settings::get()->isDepthBufferEnabled()) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    glfwSetInputMode(mainWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Uncommenting this call will result in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    engine.start();

    // std::string ss =
    // "QWERTYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY";

    return 0;
}
