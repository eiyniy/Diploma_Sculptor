#pragma once

#include <IEvent.hpp>

class ReleaseMouseEvent : public IEvent {
public:
    [[nodiscard]] EventType getType() const override;
};
