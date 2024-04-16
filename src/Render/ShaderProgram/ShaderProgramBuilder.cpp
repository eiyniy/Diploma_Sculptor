#include <ShaderProgramBuilder.hpp>

#include <BaseTextParser.hpp>
#include <IBuilder.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

ShaderProgramBuilder::ShaderProgramBuilder()
    : isInited(false)
    , isLinked(false)
    , addedAttributesCount(0)
{
}

void ShaderProgramBuilder::reset()
{
    IBuilder<ShaderProgram>::reset();

    isLinked = false;
    isInited = false;

    addedAttributesCount = 0;
    addedShadersTypes.clear();
}

bool ShaderProgramBuilder::isFinished() const
{
    return IBuilder<ShaderProgram>::isFinished() && isShadersFinished()
        && isAttributesFinished() && isLinked && isInited;
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

void ShaderProgramBuilder::init(std::string_view name)
{
    instance->init(name);
    isInited = true;
}

void ShaderProgramBuilder::addShader(
    std::string sourcePath, const GLenum shaderType)
{
    if (isLinked) {
        throw std::logic_error(
            "Can't add shader to programm builder. It's already linked");
    }

    BaseTextParser parser { std::move(sourcePath) };

    const std::string sourceStr = parser.readFile();
    const char* source = sourceStr.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &instance->success);
    if (instance->success == 0) {
        glGetShaderInfoLog(
            shader, infoLogSize, nullptr, instance->infoLog.data());
        std::cout << instance->infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED");
    }

    instance->shaders.push_back(shader);

    addedShadersTypes.insert(shaderType);
}

void ShaderProgramBuilder::addAttribute(const ShaderAttribute& attribute)
{
    instance->attributes.push_back(attribute);
    instance->attributesStride += static_cast<GLsizei>(
        attribute.getElementsCount() * attribute.getSizeofElement());

    addedAttributesCount++;
}

void ShaderProgramBuilder::link()
{
    if (!isShadersFinished()) {
        throw std::logic_error(
            "Can't link shader program. Not enought shaders");
    }

    for (auto&& shader : instance->shaders) {
        glAttachShader(instance->program, shader);
    }

    glLinkProgram(instance->program);

    glGetProgramiv(instance->program, GL_LINK_STATUS, &instance->success);
    if (instance->success == 0) {
        glGetProgramInfoLog(
            instance->program, infoLogSize, nullptr, instance->infoLog.data());
        std::cout << instance->infoLog.data() << std::endl;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    }

    for (auto&& shader : instance->shaders) {
        glDeleteShader(shader);
    }

    isLinked = true;
}
