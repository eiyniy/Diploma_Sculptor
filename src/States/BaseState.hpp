#pragma once

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <Enums.hpp>
#include <Object.hpp>

#include <map>
#include <memory>

class BaseState {
private:
    std::unique_ptr<BaseInputEngine> input;
    std::unique_ptr<BaseRenderEngine> render;

public:
    BaseState(
        std::unique_ptr<BaseInputEngine> _input,
        std::unique_ptr<BaseRenderEngine> _render);

    BaseState(const BaseState&) = delete;
    BaseState(BaseState&&) = default;
    BaseState& operator=(const BaseState&) = delete;
    BaseState& operator=(BaseState&&) = default;

    virtual ~BaseState();

    [[nodiscard]] virtual StateType getType() const = 0;

    std::unique_ptr<BaseState> update(float dt);
    void draw(const std::map<std::string, std::shared_ptr<Object>>& objects);
    // TODO: Remove objects from params
};