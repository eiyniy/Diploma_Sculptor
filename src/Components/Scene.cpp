#include <Scene.hpp>

#include <Object.hpp>

#include <string>

class BaseLightSource;

Scene::Scene() = default;

void Scene::addObject(const std::string& key, std::shared_ptr<Object> object)
{
    objects.emplace(key, object);
}

/*
Scene::~Scene()
{
    for (const auto &pair : objects)
        delete pair.second;
}

void Scene::addObject(const std::string &key, OldObject *object)
{
    if (key == floorObjectName)
        throw std::invalid_argument("This object name is reserved!");

    selectedObjectName = key;

    objects[key] = object;
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
*/
