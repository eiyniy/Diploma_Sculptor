#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>
#include <Types.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

class Scene
{
private:
    ObjInfo &cObjInfo;
    ObjInfo cFloor;
    const CoordinateVector &up;
    const double moveSpeed;

    std::vector<Vertex> floorVertices;
    std::vector<Vertex> objInfoVertices;
    CoordinateVector worldShift;
    Camera &camera;

public:
    const int defaultFps = 165;
    const double defaultFrameTime = (1.f * 1000.f / 60);

    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up, const double p_moveSpeed);

    void generateFloor(const int size, const int step);

    CoordinateVector getMoveConvert(AxisName axis, Direction direction, int dt);

    void modelConvert(
        const std::vector<Vertex> &vertices,
        std::vector<Vertex> &drawVertices,
        const std::optional<CoordinateVector> &moveConvert = std::nullopt);

    void centralizeCamera();
    void rotateCamera(const AxisName axisName, const double angle);
    void rotateCameraAround(const AxisName axisName, const double angle);
    void moveCamera(const CoordinateVector &transition);

    const ObjInfo &cGetObjInfo() const;
    std::vector<Vertex> &getObjInfoVertices();
    const std::vector<Vertex> &cGetObjInfoVertices() const;
    const std::vector<Vertex> &cGetObjInfoVerticesCopy() const;

    const ObjInfo &cGetFloor() const;
    std::vector<Vertex> &getFloorVertices();
    const std::vector<Vertex> &cGetFloorVertices() const;
    const std::vector<Vertex> &cGetFloorVerticesCopy() const;

    const Camera &cGetCamera() const;
    const CoordinateVector &cGetWorldShift() const;

    Camera &getCamera();
    CoordinateVector &getWorldShift();
};
