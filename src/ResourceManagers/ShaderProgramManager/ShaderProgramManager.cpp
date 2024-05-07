#include <ShaderProgramManager.hpp>

#include <IUniformLoader.hpp>
#include <LinesUniformLoader.hpp>
#include <ModelUniformLoader.hpp>
#include <ShaderAttribute.hpp>
#include <ShaderProgram.hpp>
#include <ShaderProgramBuilder.hpp>

#include <matrix_float4x4.hpp>

#include <GL/glew.h>

#include <utility>

class Camera;

ShaderProgramManager::ShaderProgramManager(
    const std::string_view _shaderFolderPath)
    : shaderFolderPath(_shaderFolderPath)
{
}

std::shared_ptr<ShaderProgram>
ShaderProgramManager::getShaderProgram(const std::string_view name) const
{
    return shaderPrograms.at(name);
}

void ShaderProgramManager::loadModelShaderProgram(
    const std::shared_ptr<glm::mat4>& modelMat,
    const std::shared_ptr<glm::mat4>& viewMat,
    const std::shared_ptr<glm::mat4>& projectionMat,
    const std::shared_ptr<Camera>& camera)
{
    shaderProgramBuilder.create();

    auto modelUniformLoader = std::make_unique<ModelUniformLoader>(
        modelMat, viewMat, projectionMat, camera);

    shaderProgramBuilder.init(modelSPName, std::move(modelUniformLoader));

    shaderProgramBuilder.addShader(
        shaderFolderPath + R"(\base.vert)", GL_VERTEX_SHADER);
    shaderProgramBuilder.addShader(
        shaderFolderPath + R"(\flat\flat.frag)", GL_FRAGMENT_SHADER);
    // shaderProgramBuilder.addShader(
    //     shaderFolderPath + R"(\normals\normals.frag)",
    //     GL_FRAGMENT_SHADER);
    // shaderProgramBuilder.addShader(
    //     R"(\normals\flatNormals.frag)",
    //     GL_FRAGMENT_SHADER);

    shaderProgramBuilder.link();

    shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
        ShaderProgram::positionAName,
        0,
        4,
        GL_FLOAT,
        sizeof(GLfloat),
        GL_FALSE));
    shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
        ShaderProgram::normalAName, 1, 3, GL_FLOAT, sizeof(GLfloat), GL_FALSE));

    // TODO: Add loading state before parsing

    shaderProgramBuilder.addNewUniform(ShaderProgram::modelUName);
    shaderProgramBuilder.addNewUniform(ShaderProgram::viewUName);
    shaderProgramBuilder.addNewUniform(ShaderProgram::projectionUName);

    shaderProgramBuilder.addNewUniform(ShaderProgram::cameraPosUName);

    shaderProgramBuilder.addNewUniform(ShaderProgram::lightColorUName);

    shaderProgramBuilder.addNewUniform(ShaderProgram::dimmingFactorUName);
    shaderProgramBuilder.addNewUniform(ShaderProgram::isDistanceDimmingUName);

    shaderPrograms[modelSPName] = shaderProgramBuilder.build();
}

void ShaderProgramManager::loadLinesShaderProgram(
    const std::shared_ptr<glm::mat4>& modelMat,
    const std::shared_ptr<glm::mat4>& viewMat,
    const std::shared_ptr<glm::mat4>& projectionMat)
{
    shaderProgramBuilder.create();

    auto linesUniformLoader = std::make_unique<LinesUniformLoader>(
        modelMat, viewMat, projectionMat);

    shaderProgramBuilder.init(linesSPName, std::move(linesUniformLoader));

    shaderProgramBuilder.addShader(
        shaderFolderPath + R"(\lines\lines.vert)", GL_VERTEX_SHADER);
    shaderProgramBuilder.addShader(
        shaderFolderPath + R"(\lines\lines.frag)", GL_FRAGMENT_SHADER);

    shaderProgramBuilder.link();

    shaderProgramBuilder.addAttribute(std::make_unique<ShaderAttribute>(
        ShaderProgram::positionAName,
        0,
        4,
        GL_FLOAT,
        sizeof(GLfloat),
        GL_FALSE));

    shaderProgramBuilder.addNewUniform(ShaderProgram::modelUName);
    shaderProgramBuilder.addNewUniform(ShaderProgram::viewUName);
    shaderProgramBuilder.addNewUniform(ShaderProgram::projectionUName);

    shaderProgramBuilder.addNewUniform(ShaderProgram::colorUName);

    shaderPrograms[linesSPName] = shaderProgramBuilder.build();
}
