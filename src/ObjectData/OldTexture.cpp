#include <OldTexture.hpp>

#include <vector_float4.hpp>

#include <vector>

OldTexture::OldTexture(
    const int _width, const int _height, const std::vector<glm::vec4>& _data)
    : width(_width)
    , height(_height)
    , data(_data)
{
}

int OldTexture::cGetWidth() const { return width; }

int OldTexture::cGetHeight() const { return height; }

const std::vector<glm::vec4>& OldTexture::cGetData() const { return data; }
