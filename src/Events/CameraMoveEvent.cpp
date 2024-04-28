#include <CameraMoveEvent.hpp>

#include <Enums.hpp>

CameraMoveEvent::CameraMoveEvent(
    const AxisName _axisName, const Direction _direction)
    : axisName(_axisName)
    , direction(_direction)
{
}

EventType CameraMoveEvent::getType() const { return EventType::CameraMove; }

AxisName CameraMoveEvent::getAxisName() const { return axisName; }

Direction CameraMoveEvent::getDirection() const { return direction; }
