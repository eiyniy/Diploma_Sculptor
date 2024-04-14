#include <Engine.hpp>

#include <Camera.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>
#include <Object.hpp>
#include <Scene.hpp>
#include <Settings.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <mat4x4.hpp>
#include <matrix_clip_space.hpp>
#include <qualifier.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

// TODO: Replace with smth normal
const auto* const shaderProgramName = "base";

Engine::Engine(
    Scene _scene, std::unique_ptr<MainWindow> _mainWindow, Camera _camera)
    : scene(std::move(_scene))
    , mainWindow(std::move(_mainWindow))
    , camera(_camera)
    , moveAxis(AxisName::X)
    , moveDirection(Direction::Forward)
    , deltaTime(0.F)
    , lastFrameTime(0.F)
    , modelMat(1)
    , viewMat(1)
    , projectionMat(1)
{
    projectionMat = glm::perspective(
        camera.cGetFOV(),
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

    auto shaderProgram = std::make_unique<ShaderProgram>(shaderProgramName);
    shaderProgram->addShader(
        R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\base.vert)",
        GL_VERTEX_SHADER);
    shaderProgram->addShader(
        R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\resources\shaders\base.frag)",
        GL_FRAGMENT_SHADER);

    shaderProgram->link();

    auto object = std::make_shared<Object>(
        objectVertices, std::nullopt, objectTextureVertices, std::nullopt);

    object->addShader(std::move(shaderProgram));

    auto containerTexture = std::make_unique<Texture>(
        "containerTexture", GL_TEXTURE0, GL_TEXTURE_2D);
    auto faceTexture
        = std::make_unique<Texture>("faceTexture", GL_TEXTURE1, GL_TEXTURE_2D);

    containerTexture->bind();
    containerTexture->setDefaults();
    containerTexture->load("C:/Users/Natallia/Documents/Labs/Diploma/"
                           "Diploma_Sculptor/resources/textures/container.jpg");
    containerTexture->unbind();

    faceTexture->bind();
    faceTexture->setDefaults();
    faceTexture->load("C:/Users/Natallia/Documents/Labs/Diploma/"
                      "Diploma_Sculptor/resources/textures/awesomeface.png");
    faceTexture->unbind();

    object->addTexture(std::move(containerTexture));
    object->addTexture(std::move(faceTexture));

    object->enableShader(shaderProgramName);
    object->bindTextures();
    object->disableCurrentShader();

    object->setupVAO();

    scene.addObject("OBJECT", object);

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
        camera.move(AxisName::Z, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_S]) {
        camera.move(AxisName::Z, Direction::Backward, deltaTime);
    }
    if (keys[GLFW_KEY_D]) {
        camera.move(AxisName::X, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_A]) {
        camera.move(AxisName::X, Direction::Backward, deltaTime);
    }
    if (keys[GLFW_KEY_SPACE]) {
        camera.move(AxisName::Y, Direction::Forward, deltaTime);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        camera.move(AxisName::Y, Direction::Backward, deltaTime);
    }

    camera.rotate(coordOffset);
}

void Engine::draw()
{
    MainWindow::clear();

    viewMat = camera.cGetViewMat();

    auto& objects = scene.getAllObjects();

    for (auto&& object : objects) {
        object.second->enableShader(shaderProgramName);
        object.second->loadTransformMatrices(modelMat, viewMat, projectionMat);
        object.second->draw();
        object.second->disableCurrentShader();
    }

    mainWindow->swapBuffers();
}
