#pragma once

#include <Object.hpp>

#include <map>
#include <memory>
#include <string>

class Object;
class Camera;
class BaseLightSource;

class Scene {
private:
    // std::string selectedObjectName;
    // std::map<std::string, OldObject *> objects;

    // BaseLightSource *lightSource;

    std::map<std::string, std::shared_ptr<Object>> objects;

public:
    // const std::string floorObjectName = "_FLOOR";

    Scene();

    // ~Scene();

    void addObject(const std::string& key, std::shared_ptr<Object> object);

    std::shared_ptr<Object> getObject(const std::string& key);

    std::map<std::string, std::shared_ptr<Object>>& getAllObjects();

    /*
    [[nodiscard]] const OldObject *cGetObject(const std::string &key) const;

    [[nodiscard]] std::vector<std::string> cGetAllObjectNames() const;

    [[nodiscard]] const std::string &cGetSelectedObjectName() const;

    [[nodiscard]] const BaseLightSource *cGetLightSource() const;

    OldObject *getObject(const std::string &key);

    Camera &getCamera();
    */
};

/*
inline const OldObject *Scene::cGetObject(const std::string &key) const
{
    return objects.at(key);
}

inline OldObject *Scene::getObject(const std::string &key)
{
    return objects.at(key);
}
*/

inline std::shared_ptr<Object> Scene::getObject(const std::string& key)
{
    return objects.at(key);
}

inline std::map<std::string, std::shared_ptr<Object>>& Scene::getAllObjects()
{
    return objects;
}
