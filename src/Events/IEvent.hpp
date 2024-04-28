#pragma once

#include <Enums.hpp>

class IEvent {
public:
    IEvent() = default;

    IEvent(const IEvent&) = default;
    IEvent(IEvent&&) = delete;
    IEvent& operator=(const IEvent&) = default;
    IEvent& operator=(IEvent&&) = delete;

    virtual ~IEvent() = default;

    [[nodiscard]] virtual EventType getType() const = 0;
};