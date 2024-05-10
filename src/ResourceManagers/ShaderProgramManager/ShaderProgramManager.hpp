#pragma once

#include "MainWindow.hpp"
#include <ShaderProgram.hpp>
#include <ShaderProgramBuilder.hpp>

#include <matrix_float4x4.hpp>

#include <map>
#include <memory>
#include <string>
#include <string_view>

class Camera;

using namespace std::string_view_literals;

class ShaderProgramManager {
private:
    ShaderProgramBuilder shaderProgramBuilder;
    std::string shaderFolderPath;

    std::map<std::string_view, std::shared_ptr<ShaderProgram>> shaderPrograms;

public:
    static constexpr auto modelSPName = "MODEL"sv;
    static constexpr auto linesSPName = "LINES"sv;
    static constexpr auto loadingSPName = "LOADING"sv;

    ShaderProgramManager(std::string_view _shaderFolderPath);

    ShaderProgramManager(const ShaderProgramManager&) = delete;
    ShaderProgramManager(ShaderProgramManager&&) = default;
    ShaderProgramManager& operator=(const ShaderProgramManager&) = delete;
    ShaderProgramManager& operator=(ShaderProgramManager&&) = default;

    ~ShaderProgramManager() = default;

    [[nodiscard]] std::shared_ptr<ShaderProgram>
    getShaderProgram(std::string_view name) const;

    void loadModelShaderProgram(
        const std::shared_ptr<glm::mat4>& modelMat,
        const std::shared_ptr<glm::mat4>& viewMat,
        const std::shared_ptr<glm::mat4>& projectionMat,
        const std::shared_ptr<Camera>& camera);

    void loadLinesShaderProgram(
        const std::shared_ptr<glm::mat4>& modelMat,
        const std::shared_ptr<glm::mat4>& viewMat,
        const std::shared_ptr<glm::mat4>& projectionMat);

    void
    loadLoadingShaderProgram(const std::shared_ptr<MainWindow>& mainWindow);
};
