#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <MainWindow.hpp>
#include <Point.hpp>
#include <ShaderProgramm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <filesystem>

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    MainWindow mainWindow{{1280, 720}};

    // Set the required callback functions
    mainWindow.setKeyCallback(key_callback);

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    ShaderProgram shaderProgram;

    // const auto s = std::filesystem::current_path().string();

    // shaderProgram.addShader("../../../resources/shaders/base.vert", GL_VERTEX_SHADER);
    // shaderProgram.addShader("../../../resources/shaders/base.frag", GL_FRAGMENT_SHADER);

    shaderProgram.addShader("C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/shaders/base.vert", GL_VERTEX_SHADER);
    shaderProgram.addShader("C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/shaders/base.frag", GL_FRAGMENT_SHADER);

    shaderProgram.link();

    /*
    Сарай
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f, 1.f,   // Top Right
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f, 0.f,  // Bottom Right
        -0.5f, -0.5f, 0.0f, 1.f, 1.f, 0.f, 0.f, 0.f, // Bottom Left
        -0.5f, 0.5f, 0.0f, 1.f, 0.f, 0.f, 0.f, 1.f,  // Top Left
        0.f, 1.f, 0.0f, 0.f, 0.f, 1.f, 0.5f, 0.5f,   // Top Top
    };
    */

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

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    GLuint indices[] = {
        // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3, // Second Triangle
        3, 4, 0  // Second Triangle
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    /* Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    */

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindVertexArray(0);

    GLuint texture;
    glGenTextures(1, &texture);

    // Активируем текстурный блок перед привязкой текстуры
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char *image = SOIL_load_image(
        "C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/textures/container.jpg",
        &width,
        &height,
        0,
        SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texture2;
    glGenTextures(1, &texture2);

    // Активируем текстурный блок перед привязкой текстуры
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2;
    unsigned char *image2 = SOIL_load_image(
        "C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/textures/awesomeface.png",
        &width2,
        &height2,
        0,
        SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Uncommenting this call will result in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*
    glm::mat4 modelMat{1};
    modelMat = glm::rotate(modelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    */

    glm::mat4 viewMat{1};
    // Обратите внимание, что мы смещаем сцену в направлении обратном тому, в котором мы хотим переместиться
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, -0.0f, -3.0f));

    glm::mat4 projectionMat{1};
    projectionMat = glm::perspective(glm::radians(45.0f), mainWindow.getAspect(), 0.1f, 100.0f);

    // Game loop
    while (!mainWindow.shouldClose())
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // glm::mat4 trans{1};
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // glm::mat4 modelMat{1};
        // modelMat = glm::rotate(modelMat, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // Render
        // Clear the colorbuffer
        mainWindow.clear();

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram.get(), "ourTexture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shaderProgram.get(), "ourTexture2"), 1);

        // Draw our first triangle
        shaderProgram.use();

        GLint modelLoc = glGetUniformLocation(shaderProgram.get(), "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram.get(), "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram.get(), "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

        glBindVertexArray(VAO);
        for (GLuint i = 0; i < 10; i++)
        {
            glm::mat4 modelMat{1};
            modelMat = glm::translate(modelMat, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            if (i % 3 == 0) // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * (i + 1) * 10.0f;
            modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        mainWindow.swapBuffers();
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    /*
    if (argc < 1)
        throw std::logic_error("Too few arguments passed to the program.");

    auto parser = ObjParser(argv[1]);

    auto objInfoPtFuture = std::async(
        std::launch::async,
        [&parser]
        {
            return parser.parse();
        });

    // TODO
    // const auto videoMode = sf::VideoMode::getDesktopMode();
    Point cameraResolution{1080, 720};
    Vector<4> cameraPosition{15, 40, 15};
    Camera camera{{0, 1, 0}, cameraPosition, {0, 2, 0}, cameraResolution, 100};
    Vector<4> lightPos{10, 15, 50};
    // Vector<4> lightDirection{-0.5, -2, -1};
    // lightDirection.normalize();
    // DirectLight lightSource{lightDirection};
    SpotLight lightSource{lightPos};
    // LambertModel lightingModel{};
    // PhongModel lightingModel{0.1, 1, 32};
    Scene scene{camera, &lightSource, 2, 2};

    // TODO
    // MainWindow mainWindow{cameraResolution, &lightingModel, scene.cGetLightSource(), ShadingModel::Phong};
    MainWindow mainWindow{cameraResolution};

    Sculptor sculptor{50};
    Engine engine{scene, mainWindow, sculptor};

    auto objInfoPt = objInfoPtFuture.get();
    scene.addObject("MainObject", objInfoPt);

    // engine.start();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    */
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
