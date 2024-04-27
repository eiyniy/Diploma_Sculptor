#include <BaseState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>

BaseState::BaseState(
    std::unique_ptr<BaseInputEngine> _input,
    std::unique_ptr<BaseRenderEngine> _render)
    : input(std::move(_input))
    , render(std::move(_render))
{
}

BaseState::~BaseState() = default;

std::unique_ptr<BaseState> BaseState::update(const float dt)
{
    return input->update(dt);
}

void BaseState::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    render->draw(objects);
}
