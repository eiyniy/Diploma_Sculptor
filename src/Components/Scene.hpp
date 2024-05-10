#pragma once

#include <Object.hpp>

#include <map>
#include <memory>
#include <string>

class Scene {
private:
    std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>> objects;

public:
    Scene();

    void addObject(const std::string& key, std::shared_ptr<Object> object);

    std::shared_ptr<Object> getObject(const std::string& key);

    std::shared_ptr<std::map<std::string, std::shared_ptr<Object>>>
    getAllObjects();
};
