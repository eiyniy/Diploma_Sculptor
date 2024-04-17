#include <Engine.hpp>

#include <Camera.hpp>
#include <Enums.hpp>
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

#include <matrix_clip_space.hpp>
#include <qualifier.hpp>
#include <type_mat4x4.hpp>
#include <type_vec2.hpp>
#include <type_vec3.hpp>
#include <type_vec4.hpp>
#include <vector_float2.hpp>
#include <vector_float3.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <utility>
#include <vector>

// TODO: Move to settings
const auto* const shaderProgramName = "base";

Engine::Engine(
    std::unique_ptr<Scene> _scene,
    std::unique_ptr<MainWindow> _mainWindow,
    std::unique_ptr<Camera> _camera)
    : scene(std::move(_scene))
    , mainWindow(std::move(_mainWindow))
    , camera(std::move(_camera))
    , moveAxis(AxisName::X)
    , moveDirection(Direction::Forward)
    , deltaTime(0.F)
    , lastFrameTime(0.F)
    , modelMat(1)
    , viewMat(1)
    , projectionMat(1)
{
    projectionMat = glm::perspective(
        camera->cGetFOV(),
        mainWindow->getAspect(),
        Settings::get()->getZNear(),
        Settings::get()->getZFar());
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

    ObjectBuilder objectBuilder {};

    objectBuilder.create();
    objectBuilder.init(objectVertices);

    objectBuilder.addColors(objectColorVertices);
    objectBuilder.addTextureCoords(objectTextureVertices);

    objectBuilder.addTexture(std::move(containerTexture));
    objectBuilder.addTexture(std::move(faceTexture));

    objectBuilder.addShaderProgram(std::move(shaderProgramBuilder.build()));
    objectBuilder.selectShaderProgram(shaderProgramName);

    objectBuilder.merge();

    objectBuilder.setupVAO();

    std::shared_ptr<Object> object = std::move(objectBuilder.build());

    object->enableShader();
    object->bindTextures();
    object->disableShader();

    scene->addObject("OBJECT", std::move(object));

    while (!mainWindow->shouldClose()) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        update();

        draw();
    }
}

void Engine::update()
{
    const auto keys = mainWindow->cGetKeys();
    const auto coordOffset = mainWindow->resetCoordOffset();

    if (keys[GLFW_KEY_ESCAPE]) {
        mainWindow->close();
    }

    if (keys[GLFW_KEY_W]) {
        camera->move(AxisName::Z, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_S]) {
        camera->move(AxisName::Z, Direction::Backward, deltaTime);
    }
    if (keys[GLFW_KEY_D]) {
        camera->move(AxisName::X, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_A]) {
        camera->move(AxisName::X, Direction::Backward, deltaTime);
    }
    if (keys[GLFW_KEY_SPACE]) {
        camera->move(AxisName::Y, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        camera->move(AxisName::Y, Direction::Backward, deltaTime);
    }

    camera->rotate(coordOffset);
}

void Engine::draw()
{
    MainWindow::clear();

    viewMat = camera->cGetViewMat();

    auto& objects = scene->getAllObjects();

    for (auto&& object : objects) {
        object.second->enableShader();
        object.second->loadTransformMatrices(modelMat, viewMat, projectionMat);
        object.second->draw();
        object.second->disableShader();
    }

    mainWindow->swapBuffers();
}
