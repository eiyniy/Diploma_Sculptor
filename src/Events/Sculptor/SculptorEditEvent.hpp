#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

#include <utility>

class SculptorEditEvent : public IEvent {
private:
    std::pair<float, float> mousePos;
    bool _isInverted;

public:
    SculptorEditEvent(std::pair<float, float> _mousePos, bool _isInverted);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] std::pair<float, float> getMousePos() const;

    [[nodiscard]] bool isInverted() const;
};
