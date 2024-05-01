#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

#include <utility>

class SculptorEditEvent : public IEvent {
private:
    std::pair<float, float> mousePos;

public:
    SculptorEditEvent(std::pair<float, float> _mousePos);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] std::pair<float, float> getMousePos() const;
};
