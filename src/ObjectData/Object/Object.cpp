#include <Object.hpp>

#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <qualifier.hpp>
#include <type_vec3.hpp>
#include <vector_float3.hpp>
#include <vector_float4.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

Object::Object(ConstructorPasskey<Object>&& passkey)
    : VAO(0)
    , verticesVBO(0)
    , tVerticesVBO(0)
    , nVerticesVBO(0)
    , EBO(0)
    , _isAnyShaderEnabled(false)
{
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &verticesVBO);
    glDeleteBuffers(1, &tVerticesVBO);
    glDeleteBuffers(1, &nVerticesVBO);
    glDeleteBuffers(1, &EBO);
}

void Object::throwIfShaderNotEnabled(
    const std::string& message = std::string()) const
{
    if (!isAnyShaderEnabled()) {
        throw std::logic_error("No one shader is enabled. " + message);
    }
}

void Object::throwIfShaderNotSelected(const std::string& message) const
{
    if (currentShaderProgramName.empty()) {
        throw std::logic_error("No one shader is selected. " + message);
    }
}

void Object::bindTextures()
{
    throwIfShaderNotEnabled("bindTextures");

    if (textures.empty()) {
        throw std::logic_error("Can't bind Textures. Object doesn't has them");
    }

    for (auto&& texture : textures) {
        texture.second->bind();

        loadUniform(
            texture.first, texture.second->getTextureBlock() - GL_TEXTURE0);
    }
}

void Object::bindVerticesVBO()
{
    throwIfShaderNotSelected("bindVerticesVBO");

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trVertices.size() * sizeof(GLfloat)),
        trVertices.data(),
        GL_DYNAMIC_DRAW);
}

void Object::bindTVerticesVBO()
{
    throwIfShaderNotSelected("bindTVerticesVBO");

    glBindBuffer(GL_ARRAY_BUFFER, tVerticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trTVertices.size() * sizeof(GLfloat)),
        trTVertices.data(),
        GL_STATIC_DRAW);
}

void Object::bindNVerticesVBO()
{
    throwIfShaderNotSelected("bindNVerticesVBO");

    glBindBuffer(GL_ARRAY_BUFFER, nVerticesVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(trNVertices.size() * sizeof(GLfloat)),
        trNVertices.data(),
        GL_STATIC_DRAW);
}

void Object::bindIndicesEBO()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)),
        indices.data(),
        GL_STATIC_DRAW);
}

// NOLINTNEXTLINE
void Object::unbindVBO() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Object::selectShaderProgram(const std::string_view name)
{
    currentShaderProgramName = name;
}

void Object::enableShader()
{
    if (isAnyShaderEnabled()) {
        throw std::logic_error("Can't enable shader program. There is another "
                               "one enabled already");
    }

    throwIfShaderNotSelected("Can't enable shader program.");

    auto& shaderProgram = shaderPrograms.at(currentShaderProgramName);

    shaderProgram->enable();

    currentShaderProgramName = shaderProgram->getName();
    _isAnyShaderEnabled = true;
}

void Object::disableShader()
{
    if (!isAnyShaderEnabled()) {
        throw std::logic_error(
            "Can't disable shader program. No one of them is enabled");
    }

    shaderPrograms.at(currentShaderProgramName)->disable();
    _isAnyShaderEnabled = false;
}

void Object::draw() const
{
    throwIfShaderNotEnabled("draw");

    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        nullptr);

    glBindVertexArray(0);
}

void Object::performTransform(
    const std::vector<std::pair<std::size_t, glm::vec3>>& transform)
{
    for (auto&& vertexTransform : transform) {
        const auto extendedTransformPositions
            = connectedIndicesIds.at(vertexTransform.first);

        for (auto&& transformPos : extendedTransformPositions) {
            trVertices.at(transformPos * glm::vec4::length())
                += vertexTransform.second.x;
            trVertices.at(transformPos * glm::vec4::length() + 1)
                += vertexTransform.second.y;
            trVertices.at(transformPos * glm::vec4::length() + 2)
                += vertexTransform.second.z;
        }
    }
}

bool Object::isAnyShaderEnabled() const { return _isAnyShaderEnabled; }

const std::vector<GLfloat>& Object::getTrVertices() const { return trVertices; }

const std::vector<GLuint>& Object::getIndices() const { return indices; }
