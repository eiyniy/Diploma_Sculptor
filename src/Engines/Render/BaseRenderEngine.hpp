#pragma once

#include <Object.hpp>

class BaseRenderEngine {
public:
    BaseRenderEngine() = default;

    BaseRenderEngine(const BaseRenderEngine&) = delete;
    BaseRenderEngine(BaseRenderEngine&&) = delete;
    BaseRenderEngine& operator=(const BaseRenderEngine&) = delete;
    BaseRenderEngine& operator=(BaseRenderEngine&&) = delete;

    virtual ~BaseRenderEngine() = default;

    virtual void draw() = 0;
};