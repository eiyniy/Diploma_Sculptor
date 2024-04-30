#include <SculptorEditEvent.hpp>

SculptorEditEvent::SculptorEditEvent(std::pair<float, float> _mousePos)
    : mousePos(_mousePos)
{
}

EventType SculptorEditEvent::getType() const { return EventType::SculptorEdit; }

std::pair<float, float> SculptorEditEvent::getMousePos() const
{
    return mousePos;
}
