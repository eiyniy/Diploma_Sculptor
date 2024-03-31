#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <MainWindow.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stddef.h>

#include <iostream>

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Shaders
const GLchar *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";

const GLchar *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f, // Bottom Left
        -0.5f, 0.5f, 0.0f   // Top Left
    };
    GLuint indices[] = {
        // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindVertexArray(0);

    // Uncommenting this call will result in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    // while (!glfwWindowShouldClose(window))
    while (true)
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        // glfwSwapBuffers(window);
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
