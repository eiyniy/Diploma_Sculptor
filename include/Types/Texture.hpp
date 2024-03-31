#pragma once

#include <Matrix.hpp>

#include <vector>

class Texture
{
private:
    int width;
    int height;
    std::vector<Vector<4>> data;

public:
    Texture(
        const int _width,
        const int _height,
        const std::vector<Vector<4>> &_data);

    int cGetWidth() const;
    int cGetHeight() const;
    const std::vector<Vector<4>> &cGetData() const;
};

inline int Texture::cGetWidth() const
{
    return width;
}

inline int Texture::cGetHeight() const
{
    return height;
}

inline const std::vector<Vector<4>> &Texture::cGetData() const
{
    return data;
}
