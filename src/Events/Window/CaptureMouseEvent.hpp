#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

class CaptureMouseEvent : public IEvent {
public:
    [[nodiscard]] EventType getType() const override;
};
