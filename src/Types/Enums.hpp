#pragma once

enum class ObjEntryType {
    MtlPath,
    UseMtl,
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
};

enum class MtlEntryType {
    NewMaterial,
    Ambient,
    Diffuse,
    Specular,
    SpecularExp,
    DiffuseMap,
    EmissiveMap,
    NormalMap,
    MRAOMap
};

enum class AxisName { X, Y, Z };

enum class Direction { Forward, Backward };

enum class ModelMatrixConvert { Move, Rotate, Scale };

enum class SceneMatrixConvert { View, Projection, Viewport };

enum ShadingModel { Flat, Phong };

enum class TextureType { Diffuse, Normal, Emissive, MRAO };

enum class StateType { View, Edit, Close };

enum class EventType { CameraMove, CameraRotate, BrushMove, SculptorEdit };
