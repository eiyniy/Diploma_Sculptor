#pragma once

#include <Matrix.hpp>

#include <utility>

enum class AxisName;
enum class Direction;

class Camera
{
private:
    Vector<4> up;
    Vector<4> position;
    Vector<4> target;
    std::pair<int, int> resolution;

    const int fov;

public:
    Camera(
        const Vector<4> &_up,
        const Vector<4> &_position,
        const Vector<4> &_target,
        std::pair<int, int> &_resolution,
        int _fov);

    void move(const Vector<4> &transition);

    void rotateAround(
        AxisName axisName,
        Direction direction,
        double step);

    void setResolution(const std::pair<int, int> &newResolution);

    void setTarget(const Vector<4> &newTarget);

    const Vector<4> &cGetUp() const;

    const Vector<4> &cGetPosition() const;

    const Vector<4> &cGetTarget() const;

    const std::pair<int, int> &cGetResolution() const;

    int cGetFOV() const;

    double cGetAspect() const;
};

inline const Vector<4> &Camera::cGetUp() const
{
    return up;
}

inline const Vector<4> &Camera::cGetPosition() const
{
    return position;
}

inline const Vector<4> &Camera::cGetTarget() const
{
    return target;
}

inline const std::pair<int, int> &Camera::cGetResolution() const
{
    return resolution;
}

inline int Camera::cGetFOV() const
{
    return fov;
}

inline double Camera::cGetAspect() const
{
    return ((double)resolution.first) / resolution.second;
}
