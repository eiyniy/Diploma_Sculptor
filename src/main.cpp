#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Settings.hpp>

#include <iostream>
#include <memory>
#include <utility>

const std::pair<int, int> startupResolution { 1280, 720 };

// TODO: Make libraries auto downloadable

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    auto mainWindow = std::make_unique<MainWindow>(startupResolution);
    glfwSetInputMode(mainWindow->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera camera { Settings::get()->getCameraSpeed(),
                    Settings::get()->getCameraFoV() };

    Scene scene {};

    Engine engine { scene, std::move(mainWindow), camera };

    glfwSwapInterval(static_cast<int>(Settings::get()->isVSyncEnabled()));

    if (Settings::get()->isDepthBufferEnabled()) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    // Uncommenting this call will result in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    engine.start();

    return 0;
}
