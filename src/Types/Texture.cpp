#include <Texture.hpp>

#include <Matrix.hpp>

#include <vector>

Texture::Texture(
    const int _width,
    const int _height,
    const std::vector<Vector<4>> &_data)
    : width(_width),
      height(_height),
      data(_data) {}
