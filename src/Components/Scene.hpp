#pragma once

#include <Object.hpp>

#include <map>
#include <string>

class Object;
class Camera;
class BaseLightSource;

class Scene {
private:
    // std::string selectedObjectName;
    // std::map<std::string, OldObject *> objects;

    // BaseLightSource *lightSource;

    std::map<std::string, Object> objects;

public:
    // const std::string floorObjectName = "_FLOOR";

    Scene();

    // ~Scene();

    void addObject(const std::string& key, Object object);

    const Object& getObject(const std::string& key);

    const std::map<std::string, Object>& getAllObjects();

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

inline const Object& Scene::getObject(const std::string& key)
{
    return objects.at(key);
}

inline const std::map<std::string, Object>& Scene::getAllObjects()
{
    return objects;
}
