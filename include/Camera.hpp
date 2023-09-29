#pragma once

#include <CoordinateVector.hpp>

class Camera
{
private:
    CoordinateVector position;
    CoordinateVector target;

public:
    Camera(const CoordinateVector &p_position, const CoordinateVector &p_target);

    CoordinateVector &getPosition();
    CoordinateVector &getTarget();
};
