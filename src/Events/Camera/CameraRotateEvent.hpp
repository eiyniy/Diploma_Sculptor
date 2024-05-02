#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

#include <utility>

class CameraRotateEvent : public IEvent {
private:
    std::pair<float, float> coordOffset;

public:
    CameraRotateEvent(std::pair<float, float> _coordOffset);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] std::pair<float, float> getCoordOffset() const;
};
