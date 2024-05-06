#pragma once

#include <matrix_float4x4.hpp>

class ShaderProgram;

class IUniformLoader {
public:
    IUniformLoader() = default;

    IUniformLoader(const IUniformLoader&) = default;
    IUniformLoader(IUniformLoader&&) = delete;
    IUniformLoader& operator=(const IUniformLoader&) = default;
    IUniformLoader& operator=(IUniformLoader&&) = delete;

    virtual ~IUniformLoader() = default;

    virtual void load(ShaderProgram* shaderProgram) = 0;
};
