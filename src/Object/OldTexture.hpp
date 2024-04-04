#pragma once

#include <Matrix.hpp>

#include <vector>

class OldTexture
{
private:
    int width;
    int height;
    std::vector<Vector<4>> data;

public:
    OldTexture(
        const int _width,
        const int _height,
        const std::vector<Vector<4>> &_data);

    int cGetWidth() const;
    int cGetHeight() const;
    const std::vector<Vector<4>> &cGetData() const;
};

inline int OldTexture::cGetWidth() const
{
    return width;
}

inline int OldTexture::cGetHeight() const
{
    return height;
}

inline const std::vector<Vector<4>> &OldTexture::cGetData() const
{
    return data;
}
