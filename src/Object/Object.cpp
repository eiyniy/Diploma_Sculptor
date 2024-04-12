#include <Object.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <Camera.hpp>

#include <stdexcept>

Object::Object(
    const std::vector<glm::vec3>& _vertices,
    const std::optional<std::vector<glm::vec3>>& _colorVertices = std::nullopt,
    const std::optional<std::vector<glm::vec2>>& _textureVertices
    = std::nullopt,
    const std::optional<std::vector<glm::vec<3, GLuint>>>& _indices
    = std::nullopt)
    : _hasColor(_colorVertices.has_value())
    , _hasTexture(_textureVertices.has_value())
    , _hasIndices(_indices.has_value())
    , verticesSize(_vertices.size())
    , verticesUnionSize(0)
    , indicesUnionSize(0)
    , VAO(0)
    , VBO(0)
    , EBO(0)
{
    if (_hasColor && _colorVertices->size() != _vertices.size()) {
        throw std::logic_error(
            "Can't create Object: colorVertices vector has incorrect size.");
    }
    if (_hasTexture && _textureVertices->size() != _vertices.size()) {
        throw std::logic_error(
            "Can't create Object: textureVertices vector has incorrect size.");
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (_hasIndices) {
        glGenBuffers(1, &EBO);
    }

    const auto vectorSize = _vertices.size();

    verticesUnionSize = vectorSize * 3;
    verticesUnionStep = 3;

    if (_hasColor) {
        verticesUnionSize += vectorSize * 3;
        verticesUnionStep += 3;
    }
    if (_hasTexture) {
        verticesUnionSize += vectorSize * 2;
        verticesUnionStep += 2;
    }

    verticesUnion.resize(verticesUnionSize);

    for (std::size_t i = 0; i < vectorSize; ++i) {
        const auto j = i * verticesUnionStep;
        const auto& vertex = _vertices[i];

        verticesUnion[j] = vertex.x;
        verticesUnion[j + 1] = vertex.y;
        verticesUnion[j + 2] = vertex.z;

        auto k = 3;

        if (_hasColor) {
            const auto& colorVertex = _colorVertices->at(i);

            verticesUnion[j + k] = colorVertex.r;
            verticesUnion[j + k + 1] = colorVertex.g;
            verticesUnion[j + k + 2] = colorVertex.b;

            k += 3;
        }

        if (_hasTexture) {
            const auto& textureVertex = _textureVertices->at(i);

            verticesUnion[j + k] = textureVertex.x;
            verticesUnion[j + k + 1] = textureVertex.y;

            k += 2;
        }
    }

    if (_indices.has_value()) {
        indicesUnionSize = _indices->size() * 3;
        indicesUnion.resize(indicesUnionSize);

        for (std::size_t i = 0; i < _indices->size(); ++i) {
            const auto j = i * 3;
            const auto& index = _indices->at(i);

            indicesUnion[j] = index.x;
            indicesUnion[j + 1] = index.y;
            indicesUnion[j + 2] = index.z;
        }
    }

    shaderProgram.addShader(
        "C:\\Users\\Natallia\\Documents\\Labs\\Diploma\\Diploma_"
        "Sculptor\\resources\\shaders\\base.vert",
        GL_VERTEX_SHADER);
    shaderProgram.addShader(
        "C:/Users/Natallia/Documents/Labs/Diploma/Diploma_Sculptor/resources/"
        "shaders/"
        "base.frag",
        GL_FRAGMENT_SHADER);

    shaderProgram.link();
}

Object::~Object()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    if (hasIndices()) {
        glDeleteBuffers(1, &EBO);
    }
}

GLint Object::findUniform(const std::string& uniformName) const
{
    const auto uniformLocation
        = glGetUniformLocation(shaderProgram.get(), uniformName.c_str());

    if (uniformLocation == -1) {
        throw std::logic_error(
            "Can't find uniform location. Uniform name - " + uniformName);
    }

    return uniformLocation;
}

void Object::loadTransformMatrices(
    const glm::mat4& modelMat,
    const glm::mat4& viewMat,
    const glm::mat4& projectionMat) const
{
    const auto modelLoc = findUniform(ShaderProgram::defaultModelUniformName);
    const auto viewLoc = findUniform(ShaderProgram::defaultViewUniformName);
    const auto projectionLoc
        = findUniform(ShaderProgram::defaultProjectionUniformName);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(
        projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));
}

void Object::addTexture(const Texture& texture, const std::string& name)
{
    if (!textures.has_value()) {
        textures = std::map<std::string, Texture>();
    }

    textures->emplace(name, texture);
}

void Object::bindTexture(
    const std::string& name, const std::string& uniformName)
{
    shaderProgram.use();

    textures->at(name).bind();
    glUniform1i(
        glGetUniformLocation(shaderProgram.get(), uniformName.c_str()),
        textures->at(name).getTextureBlock());
}

void Object::setupVAO()
{
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    // and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verticesUnionSize),
        verticesUnion.data(),
        GL_STATIC_DRAW);

    if (hasIndices()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(indicesUnionSize),
            indicesUnion.data(),
            GL_STATIC_DRAW);
    }

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(verticesUnionStep * sizeof(GLfloat)),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    auto offset = 3;
    if (hasColor()) {
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(verticesUnionStep * sizeof(GLfloat)),
            reinterpret_cast<GLvoid*>(offset * sizeof(GLfloat))); // NOLINT
        glEnableVertexAttribArray(1);

        offset += 3;
    }

    if (hasTexture()) {
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(verticesUnionStep * sizeof(GLfloat)),
            reinterpret_cast<GLvoid*>(offset * sizeof(GLfloat))); // NOLINT
        glEnableVertexAttribArray(2);

        offset += 2;
    }

    // Note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the currently bound vertex buffer object so afterwards we can
    // safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to
    // prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to
    // this VAO
    glBindVertexArray(0);
}

void Object::draw() const
{
    glBindVertexArray(VAO);

    if (hasIndices()) {
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(indicesUnionSize),
            GL_UNSIGNED_INT,
            0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesSize));
    }

    glBindVertexArray(0);
}
