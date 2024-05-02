#include <SculptorEditEvent.hpp>

#include <Enums.hpp>

SculptorEditEvent::SculptorEditEvent(
    const std::pair<float, float> _mousePos, const bool _isInverted)
    : mousePos(_mousePos)
    , _isInverted(_isInverted)
{
}

EventType SculptorEditEvent::getType() const { return EventType::SculptorEdit; }

std::pair<float, float> SculptorEditEvent::getMousePos() const
{
    return mousePos;
}

bool SculptorEditEvent::isInverted() const { return _isInverted; }
