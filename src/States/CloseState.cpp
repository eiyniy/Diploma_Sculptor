#include <CloseState.hpp>

#include <BaseInputEngine.hpp>
#include <BaseRenderEngine.hpp>
#include <Enums.hpp>

CloseState::CloseState()
    : BaseState(nullptr, nullptr)
{
}

StateTypes CloseState::getType() const { return StateTypes::Close; }