#include <Scene.hpp>

#include <Object.hpp>

#include <utility>
#include <stdexcept>
#include <string>
#include <vector>

class BaseLightSource;
class Camera;

Scene::Scene(
    Camera &_camera,
    BaseLightSource *_lightSource,
    const double _moveSpeed,
    const double _rotationSpeed)
    : camera(_camera),
      lightSource(_lightSource),
      defaultFrameTime(1000.f / 60),
      moveSpeed(_moveSpeed),
      rotationSpeed(_rotationSpeed) {}

Scene::~Scene()
{
    for (const auto &pair : objects)
        delete pair.second;
}

void Scene::addObject(const std::string &key, Object *object)
{
    if (key == floorObjectName)
        throw std::invalid_argument("This object name is reserved!");

    selectedObjectName = key;

    objects[key] = object;

    // generateFloor();
}

std::vector<std::string> Scene::cGetAllObjectNames() const
{
    auto res = std::vector<std::string>();

    for (const auto &pair : objects)
        res.emplace_back(pair.first);

    return res;
}

const std::string &Scene::cGetSelectedObjectName() const
{
    return selectedObjectName;
}

const BaseLightSource *Scene::cGetLightSource() const
{
    return lightSource;
}
