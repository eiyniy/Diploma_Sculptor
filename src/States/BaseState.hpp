#pragma once

#include <Enums.hpp>

#include <map>
#include <memory>
#include <optional>
#include <string>

class Object;
class MainWindow;
class BaseInputEngine;
class BaseRenderEngine;

// TODO: Remove all other states
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

    void
    setWindowUserPointer(const std::shared_ptr<MainWindow>& mainWindow) const;

    std::optional<StateType> update(float dt);
    void draw(const std::map<std::string, std::shared_ptr<Object>>& objects);
    // TODO: Remove objects from params
};