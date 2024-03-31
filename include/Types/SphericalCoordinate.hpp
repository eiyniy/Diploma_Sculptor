#pragma once

enum class AxisName;
enum class Direction;

class SphericalCoordinate
{
public:
    SphericalCoordinate(
        double _r,
        double _a,
        double _b);

    void move(
        AxisName axisName,
        Direction direction,
        double step,
        bool &isCameraReversed);

    double r, a, b;
};