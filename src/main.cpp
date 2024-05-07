#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Settings.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>
#include <memory>
#include <utility>

// TODO: Make libraries auto downloadable

// TODO: Move to settings
const std::pair<int, int> startupResolution { 1280, 720 };

int main(int argc, char** argv)
{
    // TODO: Add App class & App::run
    try {
        std::cout << "Hello world!" << std::endl;

        auto mainWindow = std::make_unique<MainWindow>(startupResolution);

        auto camera = std::make_unique<Camera>(
            Settings::get()->getCameraSpeed(), Settings::get()->getCameraFoV());

        auto scene = std::make_unique<Scene>();

        Engine engine { std::move(scene),
                        std::move(mainWindow),
                        std::move(camera) };

        glfwSwapInterval(static_cast<int>(Settings::get()->isVSyncEnabled()));

        if (Settings::get()->isDepthBufferEnabled()) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        // Uncommenting this call will result in wireframe polygons.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        engine.start();
    } catch (std::exception& e) {
        std::cout << "  An exception has occurred:" << std::endl
                  << e.what() << std::endl;
    } catch (...) {
        std::cout << "  Unknown exception has occurred:" << std::endl;
    }

    return 0;
}
