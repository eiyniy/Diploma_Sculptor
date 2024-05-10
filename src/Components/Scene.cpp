#include <Scene.hpp>

#include <memory>
#include <string>

class Object;

Scene::Scene()
{
    objects
        = std::make_shared<std::map<std::string, std::shared_ptr<Object>>>();
};

void Scene::addObject(const std::string& key, std::shared_ptr<Object> object)
{
    objects->emplace(key, object);
}

std::shared_ptr<Object> Scene::getObject(const std::string& key)
{
    return objects->at(key);
}

std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>>
Scene::getAllObjects()
{
    return objects;
}
