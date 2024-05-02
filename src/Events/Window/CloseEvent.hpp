#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

class CloseEvent : public IEvent {
public:
    [[nodiscard]] EventType getType() const override;
};
