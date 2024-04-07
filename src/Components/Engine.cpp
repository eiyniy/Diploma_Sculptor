#include <Engine.hpp>

#include <MainWindow.hpp>
#include <Enums.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <ShaderProgramm.hpp>

// IWYU pragma: begin_keep
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// IWYU pragma: end_keep

#include <GLFW/glfw3.h>

#include <utility>
#include <array>

Engine::Engine(
    Scene &_scene,
    MainWindow &_mainWindow,
    Camera &_camera)
    : scene(_scene),
      mainWindow(_mainWindow),
      camera(_camera),
      moveAxis(AxisName::X),
      moveDirection(Direction::Forward),
      deltaTime(0.f),
      lastFrameTime(0.f),
      shaderProgram(),
      containerTexture{GL_TEXTURE0, GL_TEXTURE_2D},
      faceTexture{GL_TEXTURE1, GL_TEXTURE_2D}
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shaderProgram.addShader(
        "C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/shaders/base.vert",
        GL_VERTEX_SHADER);
    shaderProgram.addShader(
        "C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/shaders/base.frag",
        GL_FRAGMENT_SHADER);

    shaderProgram.link();

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindVertexArray(0);

    containerTexture.bind();

    containerTexture.setDefaults();
    containerTexture.load("C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/textures/container.jpg");

    containerTexture.unbind();

    faceTexture.bind();

    faceTexture.setDefaults();
    faceTexture.load("C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/textures/awesomeface.png");

    faceTexture.unbind();

    projectionMat = glm::perspective(
        camera.cGetFOV(),
        mainWindow.getAspect(),
        0.1f,
        100.0f);
}

Engine::~Engine()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}

void Engine::start()
{
    while (!mainWindow.shouldClose())
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glfwPollEvents();

        update();

        draw();
    }
}

void Engine::update()
{
    const auto &keys = mainWindow.cGetKeys();
    const auto coordOffset = mainWindow.resetCoordOffset();

    if (keys[GLFW_KEY_ESCAPE])
        mainWindow.close();

    if (keys[GLFW_KEY_W])
        camera.move(AxisName::Z, Direction::Forward, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.move(AxisName::Z, Direction::Backward, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.move(AxisName::X, Direction::Forward, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.move(AxisName::X, Direction::Backward, deltaTime);
    if (keys[GLFW_KEY_SPACE])
        camera.move(AxisName::Y, Direction::Forward, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camera.move(AxisName::Y, Direction::Backward, deltaTime);

    camera.rotate(coordOffset);
}

void Engine::draw()
{
    mainWindow.clear();

    const auto &viewMat = camera.cGetViewMat();
    glm::mat4 modelMat{1};

    // Render
    shaderProgram.use();

    // Bind texture
    containerTexture.bind();
    glUniform1i(glGetUniformLocation(shaderProgram.get(), "texture1"), 0);

    faceTexture.bind();
    glUniform1i(glGetUniformLocation(shaderProgram.get(), "texture2"), 1);

    // Draw our first triangle
    GLint modelLoc = glGetUniformLocation(shaderProgram.get(), "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram.get(), "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram.get(), "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Swap the screen buffers
    mainWindow.swapBuffers();
}
