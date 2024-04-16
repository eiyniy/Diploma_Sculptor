#pragma once

#include <vector_float4.hpp>

#include <vector>

class OldTexture {
private:
    int width;
    int height;
    std::vector<glm::vec4> data;

public:
    OldTexture(int _width, int _height, const std::vector<glm::vec4>& _data);

    [[nodiscard]] int cGetWidth() const;
    [[nodiscard]] int cGetHeight() const;
    [[nodiscard]] const std::vector<glm::vec4>& cGetData() const;
};

inline int OldTexture::cGetWidth() const { return width; }

inline int OldTexture::cGetHeight() const { return height; }

inline const std::vector<glm::vec4>& OldTexture::cGetData() const
{
    return data;
}
