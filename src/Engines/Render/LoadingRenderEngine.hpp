#pragma once

#include <BaseRenderEngine.hpp>

#include <GL/glew.h> // IWYU pragma: keep

#include <memory>
#include <vector>

class ShaderProgram;
class MainWindow;
class Object;

class LoadingRenderEngine : public BaseRenderEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<ShaderProgram> shaderProgram;

    GLuint dummyVBO;
    GLuint dummyVAO;
    GLuint dummyEBO;

    std::vector<GLfloat> dummyVertices;
    std::vector<GLuint> dummyIndices;

public:
    LoadingRenderEngine(
        std::shared_ptr<MainWindow> _mainWindow,
        std::shared_ptr<ShaderProgram> _shaderProgram);

    ~LoadingRenderEngine();

    void draw() override;
};
