#include <iostream>
#include <future>
#include <memory>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Timer.hpp>
#include <Matrix.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <Converter.hpp>
#include <Engine.hpp>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    auto parser = ObjParser(argv[1]);
    auto objInfoPtFuture = std::async(
        std::launch::async,
        [&parser]
        {
            auto str = parser.readFile();
            return parser.parseEntries(str);
        });

    auto videoMode = sf::VideoMode::getDesktopMode();
    auto cameraResolution = Point(videoMode.width, videoMode.height);
    auto camera = Camera({0, 1, 0}, {1, 50, 150}, {0, 0, 0}, cameraResolution, 100);
    auto scene = Scene(camera, 5, 0.5);
    auto mainWindow = MainWindow(cameraResolution);
    auto engine = Engine(scene, mainWindow);

    auto objInfoPt = objInfoPtFuture.get();

    std::cout << "Vertices count - " << objInfoPt->cGetVertices().size() << std::endl;
    std::cout << "Texture vertices count - " << objInfoPt->cGetTVertices().size() << std::endl;
    std::cout << "Normal vertices count - " << objInfoPt->cGetNVertices().size() << std::endl;
    std::cout << "Polygons count - " << objInfoPt->cGetPolygons().size() << std::endl;
    std::cout << std::endl;

    scene.addObject("MainObject", objInfoPt);

    engine.start();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    return 0;
}