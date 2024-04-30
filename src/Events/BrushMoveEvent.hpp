#pragma once

#include <IEvent.hpp>

#include <utility>

class BrushMoveEvent : public IEvent {
private:
    std::pair<float, float> mousePos;

public:
    BrushMoveEvent(std::pair<float, float> _mousePos);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] std::pair<float, float> getMousePos() const;
};
