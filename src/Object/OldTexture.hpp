#pragma once

#include <Matrix.hpp>

#include <vector>

class OldTexture {
private:
    int width;
    int height;
    std::vector<Vector<4>> data;

public:
    OldTexture(int _width, int _height, const std::vector<Vector<4>>& _data);

    [[nodiscard]] int cGetWidth() const;
    [[nodiscard]] int cGetHeight() const;
    [[nodiscard]] const std::vector<Vector<4>>& cGetData() const;
};

inline int OldTexture::cGetWidth() const { return width; }

inline int OldTexture::cGetHeight() const { return height; }

inline const std::vector<Vector<4>>& OldTexture::cGetData() const
{
    return data;
}
