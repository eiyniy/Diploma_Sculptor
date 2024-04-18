#include <Camera.hpp>
#include <Engine.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <Scene.hpp>
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
#include <vector>

const std::pair<int, int> startupResolution { 1280, 720 };

// TODO: Make libraries auto downloadable

// TODO: Move to settings
const auto* const shaderProgramName = "base";

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    auto mainWindow = std::make_unique<MainWindow>(startupResolution);
    glfwSetInputMode(mainWindow->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // NOLINTBEGIN
    std::vector<glm::vec3> objectVertices {
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f }
    };
    // NOLINTEND

    // NOLINTBEGIN
    std::vector<glm::vec3> objectColorVertices {
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f }
    };
    // NOLINTEND

    // NOLINTBEGIN
    std::vector<glm::vec2> objectTextureVertices {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },

        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },

        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },

        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },

        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },

        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f }
    };
    // NOLINTEND

    TextureBuilder textureBuilder {};

    textureBuilder.create();
    textureBuilder.init("containerTexture", GL_TEXTURE0, GL_TEXTURE_2D);

    textureBuilder.bind();
    textureBuilder.setDefaults();
    textureBuilder.load("C:/Users/Natallia/Documents/Labs/Diploma/"
                        "Diploma_Sculptor/resources/textures/container.jpg");
    textureBuilder.unbind();

    auto containerTexture = textureBuilder.build();

    textureBuilder.create();
    textureBuilder.init("faceTexture", GL_TEXTURE1, GL_TEXTURE_2D);

    textureBuilder.bind();
    textureBuilder.setDefaults();
    textureBuilder.load("C:/Users/Natallia/Documents/Labs/Diploma/"
                        "Diploma_Sculptor/resources/textures/awesomeface.png");
    textureBuilder.unbind();

    auto faceTexture = textureBuilder.build();

    ShaderProgramBuilder shaderProgramBuilder {};

    shaderProgramBuilder.create();
    shaderProgramBuilder.init(shaderProgramName);

    shaderProgramBuilder.addShader(
        R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\base.vert)",
        GL_VERTEX_SHADER);
    shaderProgramBuilder.addShader(
        R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\base.frag)",
        GL_FRAGMENT_SHADER);

    shaderProgramBuilder.link();

    shaderProgramBuilder.addAttribute(
        { "position", 0, 3, GL_FLOAT, sizeof(GLfloat), GL_FALSE });
    shaderProgramBuilder.addAttribute(
        { "color", 1, 3, GL_FLOAT, sizeof(GLfloat), GL_FALSE });
    shaderProgramBuilder.addAttribute(
        { "texCoord", 2, 2, GL_FLOAT, sizeof(GLfloat), GL_FALSE });

    shaderProgramBuilder.addNewUniform(
        ShaderProgram::defaultModelUniformName,
        shaderProgramBuilder.instance->get());
    shaderProgramBuilder.addNewUniform(
        ShaderProgram::defaultViewUniformName,
        shaderProgramBuilder.instance->get());
    shaderProgramBuilder.addNewUniform(
        ShaderProgram::defaultProjectionUniformName,
        shaderProgramBuilder.instance->get());

    shaderProgramBuilder.addNewUniform(
        containerTexture->getName(), shaderProgramBuilder.instance->get());
    shaderProgramBuilder.addNewUniform(
        faceTexture->getName(), shaderProgramBuilder.instance->get());

    auto shaderProgram = shaderProgramBuilder.build();

    ObjectBuilder objectBuilder {};

    objectBuilder.create();
    objectBuilder.init(objectVertices);

    objectBuilder.addColors(objectColorVertices);
    objectBuilder.addTextureCoords(objectTextureVertices);

    objectBuilder.addTexture(std::move(containerTexture));
    objectBuilder.addTexture(std::move(faceTexture));

    objectBuilder.addShaderProgram(std::move(shaderProgram));
    objectBuilder.selectShaderProgram(shaderProgramName);

    objectBuilder.merge();

    objectBuilder.setupVAO();

    auto object = std::move(objectBuilder.build());

    object->enableShader();
    object->bindTextures();
    object->disableShader();

    engine.addObject("OBJECT", std::move(object));

    try {
        engine.start();
    } catch (std::exception& e) {
        std::cout << "  An exception has occurred:" << std::endl
                  << e.what() << std::endl;
    }

    return 0;
}
