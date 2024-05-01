#include <BaseState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <MainWindow.hpp>

#include <utility>

class Object;
enum class StateType;

BaseState::BaseState(
    std::unique_ptr<BaseInputEngine> _input,
    std::unique_ptr<BaseRenderEngine> _render)
    : input(std::move(_input))
    , render(std::move(_render))
{
}

BaseState::~BaseState() = default;

void BaseState::setWindowUserPointer(
    const std::shared_ptr<MainWindow>& mainWindow) const
{
    mainWindow->setUserPointer(input.get());
}

std::optional<StateType> BaseState::update(const float dt)
{
    return input->update(dt);
}

void BaseState::draw(
    const std::map<std::string, std::shared_ptr<Object>>& objects)
{
    render->draw(objects);
}
