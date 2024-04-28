#include <CameraRotateEvent.hpp>

#include <Enums.hpp>

CameraRotateEvent::CameraRotateEvent(const std::pair<float, float> _coordOffset)
    : coordOffset(_coordOffset)
{
}

EventType CameraRotateEvent::getType() const { return EventType::CameraRotate; }

std::pair<float, float> CameraRotateEvent::getCoordOffset() const
{
    return coordOffset;
}
