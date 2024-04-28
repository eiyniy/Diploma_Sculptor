#include "vector_float3.hpp"
#include <EditInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <Enums.hpp>
#include <MainWindow.hpp>
#include <Sculptor.hpp>
#include <ViewState.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

EditInputEngine::EditInputEngine(
    std::shared_ptr<MainWindow> _mainWindow, std::shared_ptr<Camera> _camera)
    : ModelInputEngine(std::move(_mainWindow), std::move(_camera))
{
    getMainWindow()->releaseMouse();
}

std::unique_ptr<BaseState> EditInputEngine::update(const float dt)
{
    auto res = ModelInputEngine::update(dt);
    if (res != nullptr) {
        return std::move(res);
    }

    const auto& keys = cGetKeys();

    if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_M]
        && isKeysDelayElapsed()) {
        return std::make_unique<ViewState>(getMainWindow(), getCamera());
    }

    return nullptr;
}

void EditInputEngine::mouseCallbackInner(double xpos, double ypos)
{
    glm::vec3 rayOrig;
    glm::vec3 rayDir;
    Sculptor::getRayWorld(
        { xpos, ypos },
        getMainWindow()->getActiveResolution(),
        const glm::mat4& projection_matrix,
        const glm::mat4& view_matrix,
        rayOrig,
        rayDir);

    const auto triangleId = Sculptor::getSelectedTriangleId(
        const std::vector<GLfloat>& trVertices,
        const std::vector<GLuint>& indices,
        rayOrig,
        rayDir);

    if (triangleId.has_value()) {
        std::cout << "Selected triangle id: " << *triangleId << std::endl;
    }
}
