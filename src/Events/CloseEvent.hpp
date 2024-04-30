#pragma once

#include <IEvent.hpp>

class CloseEvent : public IEvent {
public:
    [[nodiscard]] EventType getType() const override;
};
