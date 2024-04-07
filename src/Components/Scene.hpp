#pragma once

#include <string>
#include <vector>
#include <map>

class Object;
class Camera;
class BaseLightSource;

class Scene
{
private:
    std::string selectedObjectName;
    std::map<std::string, Object *> objects;

    BaseLightSource *lightSource;

public:
    const std::string floorObjectName = "_FLOOR";

    Scene();

    ~Scene();

    void addObject(const std::string &key, Object *object);

    [[nodiscard]] const Object *cGetObject(const std::string &key) const;

    [[nodiscard]] std::vector<std::string> cGetAllObjectNames() const;

    [[nodiscard]] const std::string &cGetSelectedObjectName() const;

    [[nodiscard]] const BaseLightSource *cGetLightSource() const;

    Object *getObject(const std::string &key);

    Camera &getCamera();
};

inline const Object *Scene::cGetObject(const std::string &key) const
{
    return objects.at(key);
}

inline Object *Scene::getObject(const std::string &key)
{
    return objects.at(key);
}
