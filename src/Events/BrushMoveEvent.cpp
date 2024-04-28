#include <BrushMoveEvent.hpp>

#include <Enums.hpp>

#include <utility>

BrushMoveEvent::BrushMoveEvent(std::pair<float, float> _mousePos)
    : mousePos(_mousePos)
{
}

EventType BrushMoveEvent::getType() const { return EventType::BrushMove; }

std::pair<float, float> BrushMoveEvent::getMousePos() const { return mousePos; }
