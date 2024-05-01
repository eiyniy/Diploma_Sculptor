#pragma once

#include <IEvent.hpp>
#include <Enums.hpp>

class ReleaseMouseEvent : public IEvent {
public:
    [[nodiscard]] EventType getType() const override;
};
