#include <OldTexture.hpp>

#include <vec4.hpp>

#include <vector>

OldTexture::OldTexture(
    const int _width, const int _height, const std::vector<glm::vec4>& _data)
    : width(_width)
    , height(_height)
    , data(_data)
{
}
