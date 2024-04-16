#include <ShaderProgramBuilder.hpp>

#include <BaseTextParser.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

ShaderProgramBuilder::ShaderProgramBuilder()
    : shaderProgram(nullptr)
    , isLinked(false)
    , addedAttributesCount(0)
{
}

void ShaderProgramBuilder::reset()
{
    if (shaderProgram != nullptr) {
        throw std::logic_error(
            "Can't reset ShaderProgramBuilder. There is unfinished build.");
    }

    isLinked = false;

    addedAttributesCount = 0;
    addedShadersTypes.clear();
}

bool ShaderProgramBuilder::isAttributesFinished() const
{
    return addedAttributesCount != 0;
}

bool ShaderProgramBuilder::isShadersFinished() const
{
    return addedShadersTypes.contains(GL_VERTEX_SHADER)
        && addedShadersTypes.contains(GL_FRAGMENT_SHADER);
}

void ShaderProgramBuilder::create(std::string_view name)
{
    if (shaderProgram != nullptr) {
        throw std::logic_error("Can't create new ShaderProgram. Previous build "
                               "haven't been finished.");
    }

    shaderProgram = std::make_unique<ShaderProgram>(name);
}

std::unique_ptr<ShaderProgram> ShaderProgramBuilder::build()
{
    if (shaderProgram == nullptr || !isShadersFinished()
        || !isAttributesFinished() || !isLinked) {
        throw std::logic_error(
            "Can't build ShaderProgram. Build steps haven't been completed.");
    }

    auto result = std::move(shaderProgram);

    shaderProgram = nullptr;
    reset();

    return std::move(result);
}

void ShaderProgramBuilder::addShader(
    std::string sourcePath, const GLenum shaderType)
{
    BaseTextParser parser { std::move(sourcePath) };

    const std::string sourceStr = parser.readFile();
    const char* source = sourceStr.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderProgram->success);
    if (shaderProgram->success == 0) {
        glGetShaderInfoLog(
            shader, infoLogSize, nullptr, shaderProgram->infoLog.data());
        std::cout << shaderProgram->infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED");
    }

    shaderProgram->shaders.push_back(shader);

    addedShadersTypes.insert(shaderType);
}

void ShaderProgramBuilder::addAttribute(const ShaderAttribute& attribute)
{
    shaderProgram->attributes.push_back(attribute);
    shaderProgram->attributesStride += static_cast<GLsizei>(
        attribute.getElementsCount() * attribute.getSizeofElement());

    addedAttributesCount++;
}

void ShaderProgramBuilder::link()
{
    for (auto&& shader : shaderProgram->shaders) {
        glAttachShader(shaderProgram->program, shader);
    }

    glLinkProgram(shaderProgram->program);

    glGetProgramiv(
        shaderProgram->program, GL_LINK_STATUS, &shaderProgram->success);
    if (shaderProgram->success == 0) {
        glGetProgramInfoLog(
            shaderProgram->program,
            infoLogSize,
            nullptr,
            shaderProgram->infoLog.data());
        std::cout << shaderProgram->infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    }

    for (auto&& shader : shaderProgram->shaders) {
        glDeleteShader(shader);
    }

    isLinked = true;
}
