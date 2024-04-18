#include <Scene.hpp>

#include <string>

class Object;

Scene::Scene() = default;

void Scene::addObject(const std::string& key, std::shared_ptr<Object> object)
{
    objects.emplace(key, object);
}

std::shared_ptr<Object> Scene::getObject(const std::string& key)
{
    return objects.at(key);
}

std::map<std::string, std::shared_ptr<Object>>& Scene::getAllObjects()
{
    return objects;
}
