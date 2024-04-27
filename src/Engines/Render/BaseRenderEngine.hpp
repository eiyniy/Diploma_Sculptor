#pragma once

#include <Object.hpp>

#include <map>
#include <memory>
#include <string>

class BaseRenderEngine {
public:
    BaseRenderEngine() = default;

    BaseRenderEngine(const BaseRenderEngine&) = delete;
    BaseRenderEngine(BaseRenderEngine&&) = delete;
    BaseRenderEngine& operator=(const BaseRenderEngine&) = delete;
    BaseRenderEngine& operator=(BaseRenderEngine&&) = delete;

    virtual ~BaseRenderEngine() = default;

    virtual void draw(const std::map<std::string, std::shared_ptr<Object>>& objects)
        = 0;
};