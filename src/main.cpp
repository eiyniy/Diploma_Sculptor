#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <ObjParseResult.hpp>
#include <ObjParser.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <Scene.hpp>
#include <Settings.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>
#include <ShaderProgramBuilder.hpp>
#include <Triangle.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// TODO: Make libraries auto downloadable

// TODO: Move to settings
const std::pair<int, int> startupResolution { 1280, 720 };
const auto* const shaderProgramName = "base";

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

        ShaderProgramBuilder shaderProgramBuilder {};

        shaderProgramBuilder.create();
        shaderProgramBuilder.init(shaderProgramName);

        shaderProgramBuilder.addShader(
            R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\flat\flat.vert)",
            GL_VERTEX_SHADER);
        shaderProgramBuilder.addShader(
            R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\flat\flat.frag)",
            GL_FRAGMENT_SHADER);

        shaderProgramBuilder.link();

        shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
            ShaderProgram::positionAName,
            0,
            4,
            GL_FLOAT,
            sizeof(GLfloat),
            GL_FALSE));
        shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
            ShaderProgram::normalAName,
            1,
            3,
            GL_FLOAT,
            sizeof(GLfloat),
            GL_FALSE));

        // TODO: Add loading state before parsing

        shaderProgramBuilder.addNewUniform(ShaderProgram::modelUName);
        shaderProgramBuilder.addNewUniform(ShaderProgram::viewUName);
        shaderProgramBuilder.addNewUniform(ShaderProgram::projectionUName);

        shaderProgramBuilder.addNewUniform(ShaderProgram::cameraPosUName);

        shaderProgramBuilder.addNewUniform(ShaderProgram::lightColorUName);

        shaderProgramBuilder.addNewUniform(ShaderProgram::dimmingFactorUName);
        shaderProgramBuilder.addNewUniform(
            ShaderProgram::isDistanceDimmingUName);

        auto shaderProgram = shaderProgramBuilder.build();

        const std::string projectPath
            = R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\)";

        const std::string path
            // = projectPath + R"(resources\baseModels\cube\cube.obj)";
            // = projectPath + R"(resources\baseModels\sphere\sphere.obj)";
            // = projectPath + R"(resources\models\woman1.obj)";
            = projectPath + R"(resources\models\car\car.obj)";
        // = projectPath + R"(resources\models\angel\angel.obj)";

        ObjParser parser { path };

        auto parseResult = parser.parse();

        ObjectBuilder objectBuilder {};

        const auto objectBuilderStart
            = std::chrono::high_resolution_clock::now();

        objectBuilder.create();
        objectBuilder.init(
            std::move(parseResult.getVertices()),
            std::move(parseResult.getTriangles()));

        objectBuilder.addNVertices(std::move(parseResult.getNVertices()));

        objectBuilder.addShaderProgram(std::move(shaderProgram));
        objectBuilder.selectShaderProgram(shaderProgramName);

        const auto transformStart = std::chrono::high_resolution_clock::now();

        objectBuilder.transform();

        const auto setupVAOStart = std::chrono::high_resolution_clock::now();

        objectBuilder.setupVAO();

        auto object = std::move(objectBuilder.build());

        std::cout << "Object builded." << std::endl;
        std::cout << "Start stuff time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         transformStart - objectBuilderStart)
                  << std::endl;
        std::cout << "Transform time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         setupVAOStart - transformStart)
                  << std::endl;
        std::cout << "Setup VAO time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now()
                         - setupVAOStart)
                  << std::endl;

        engine.addObject("OBJECT", std::move(object));

        engine.start();
    } catch (std::exception& e) {
        std::cout << "  An exception has occurred:" << std::endl
                  << e.what() << std::endl;
    } catch (...) {
        std::cout << "  Unknown exception has occurred:" << std::endl;
    }

    return 0;
}
