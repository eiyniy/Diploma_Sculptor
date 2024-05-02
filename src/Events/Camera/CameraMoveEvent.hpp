#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>

class CameraMoveEvent : public IEvent {
private:
    AxisName axisName;
    Direction direction;

public:
    CameraMoveEvent(AxisName _axisName, Direction _direction);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] AxisName getAxisName() const;
    [[nodiscard]] Direction getDirection() const;
};
