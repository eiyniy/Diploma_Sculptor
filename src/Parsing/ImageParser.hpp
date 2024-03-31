#pragma once

#include <Texture.hpp>

#include <CImg.h>

#include <string>
#include <memory>

enum class TextureType;

namespace cimg = cimg_library;

class ImageParser
{
private:
    const std::string path;
    const TextureType type;

public:
    ImageParser(const std::string &_path, const TextureType _type);

    std::unique_ptr<const Texture> parse() const;
};
