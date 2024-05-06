#pragma once

#include <IEvent.hpp>

class EngineLoadEvent : public IEvent {
public:
    EngineLoadEvent() = default;

    [[nodiscard]] EventType getType() const override;
};
