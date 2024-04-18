#include <Engine.hpp>

#include <Enums.hpp>
#include <InputEngine.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <RenderEngine.hpp>
#include <Scene.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>
#include <ShaderProgramBuilder.hpp>
#include <Texture.hpp>
#include <TextureBuilder.hpp>

#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

// TODO: Move to settings
const auto* const shaderProgramName = "base";

Engine::Engine(
    std::unique_ptr<Scene> _scene,
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<Camera> _camera)
    : scene(std::move(_scene))
    , mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , moveAxis(AxisName::X)
    , moveDirection(Direction::Forward)
    , deltaTime(0.F)
    , lastFrameTime(0.F)
    , renderEngine { mainWindow, camera }
    , inputEngine(mainWindow, camera)
{
}

Engine::~Engine() { glfwTerminate(); }

void Engine::start()
{
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

    std::shared_ptr<Object> object = std::move(objectBuilder.build());

    object->enableShader();
    object->bindTextures();
    object->disableShader();

    scene->addObject("OBJECT", std::move(object));

    while (!renderEngine.shouldClose()) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        update();

        draw();
    }
}

void Engine::update() { inputEngine.update(deltaTime); }

void Engine::draw()
{
    auto& objects = scene->getAllObjects();

    renderEngine.draw(objects);
}
