#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <ObjParser.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <Scene.hpp>
#include <Sculptor.hpp>
#include <Settings.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>
#include <ShaderProgramBuilder.hpp>
#include <Texture.hpp>
#include <TextureBuilder.hpp>

#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>
#include <memory>
#include <utility>

// TODO: Make libraries auto downloadable

// TODO: Move to settings
const std::pair<int, int> startupResolution { 1280, 720 };
const auto* const shaderProgramName = "base";

int main(int argc, char** argv)
{
    // TODO: Add App class & App::run
    // TODO: Add Event bus for model editing, camera move and etc
    try {
        std::cout << "Hello world!" << std::endl;

        /*
        glm::vec3 rayOrig { 0.4, 1, 0 };
        glm::vec3 rayDir { 0, -1, 0 };
        std::array<glm::vec3, 3> vertices {
            { { 1.F, 0.F, 1.F }, { -1.F, 0.F, 1.F }, { 0.F, 0.F, -1.F } }
        };
        glm::vec3 intersection;
        auto res = Sculptor::intersectRayTriangleGLM(
            rayOrig, glm::normalize(rayDir), vertices, intersection);

        const auto* resStr = res ? "true" : "false";

        std::cout << "GLM: " << std::endl;
        std::cout << "intersection: " << resStr << ": " << intersection.x << ' '
                  << intersection.y << ' ' << intersection.z << std::endl;

        res = Sculptor::intersectRayTriangleOrig(
            { rayOrig.x, rayOrig.y, rayOrig.z },
            { rayDir.x, rayDir.y, rayDir.z },
            { vertices[0].x, vertices[0].y, vertices[0].z },
            { vertices[1].x, vertices[1].y, vertices[1].z },
            { vertices[2].x, vertices[2].y, vertices[2].z },
            intersection.x,
            intersection.y,
            intersection.z);

        resStr = res ? "true" : "false";

        std::cout << "Orig: " << std::endl;
        std::cout << "intersection: " << resStr << ": " << intersection.x << ' '
                  << intersection.y << ' ' << intersection.z << std::endl;
        */

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

        // glShadeModel(GL_FLAT);

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

        const auto* const path
            // =
            // R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\baseModels\cube\cube.obj)";
            // =
            // R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\models\car\car.obj)";
            = R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\baseModels\sphere\sphere.obj)";
        // =
        // R"(C:\Users\Natallia\Documents\Labs\AKG\L1\resources\models\angel.obj)";
        // =
        // R"(C:\Users\Natallia\Documents\Labs\AKG\L1\resources\models\woman1.obj)";

        ObjParser parser { path };

        auto parseResult = parser.parse();

        ObjectBuilder objectBuilder {};

        std::cout << "parsed" << std::endl;

        objectBuilder.create();
        objectBuilder.init(
            std::move(parseResult.getVertices()),
            std::move(parseResult.getTriangles()));

        objectBuilder.addNVertices(std::move(parseResult.getNVertices()));

        objectBuilder.addShaderProgram(std::move(shaderProgram));
        objectBuilder.selectShaderProgram(shaderProgramName);

        objectBuilder.merge();

        objectBuilder.setupVAO();

        auto object = std::move(objectBuilder.build());

        std::cout << "object builded" << std::endl;

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
