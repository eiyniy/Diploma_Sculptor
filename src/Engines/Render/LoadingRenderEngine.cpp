#include <LoadingRenderEngine.hpp>

#include <BaseInputEngine.hpp>
#include <MainWindow.hpp>
#include <ShaderProgram.hpp>

#include <utility>

class Object;

LoadingRenderEngine::LoadingRenderEngine(
    std::shared_ptr<MainWindow> _mainWindow,
    std::shared_ptr<ShaderProgram> _shaderProgram)
    : mainWindow(std::move(_mainWindow))
    , shaderProgram(std::move(_shaderProgram))
    , dummyVBO(0)
    , dummyVAO(0)
    , dummyEBO(0)
{
    glGenVertexArrays(1, &dummyVAO);
    glGenBuffers(1, &dummyVBO);
    glGenBuffers(1, &dummyEBO);

    dummyVertices = { 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0 };
    dummyIndices = { 0, 1, 3, 1, 2, 3 };

    glBindVertexArray(dummyVAO);

    glBindBuffer(GL_ARRAY_BUFFER, dummyVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(dummyVertices.size() * sizeof(GLfloat)),
        dummyVertices.data(),
        GL_STATIC_DRAW);

    // FIXME: Double enableAttribute
    shaderProgram->enableAttribute(ShaderProgram::positionAName);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dummyEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(dummyIndices.size() * sizeof(GLuint)),
        dummyIndices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);
}

LoadingRenderEngine::~LoadingRenderEngine()
{
    glDeleteVertexArrays(1, &dummyVAO);
    glDeleteBuffers(1, &dummyVBO);
    glDeleteBuffers(1, &dummyEBO);
}

void LoadingRenderEngine::draw()
{
    mainWindow->clear();

    shaderProgram->enable();
    shaderProgram->loadUniforms();

    glBindVertexArray(dummyVAO);
    glDrawElements(
        GL_TRIANGLES,
        // static_cast<GLsizei>(dummyIndices.size()),
        static_cast<GLsizei>(dummyIndices.size()),
        GL_UNSIGNED_INT,
        nullptr);
    glBindVertexArray(0);

    shaderProgram->disable();

    mainWindow->swapBuffers();
}
