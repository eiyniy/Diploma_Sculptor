#pragma once

#include <OldTexture.hpp>

#include <CImg.h>

#include <memory>
#include <string>

enum class TextureType;

namespace cimg = cimg_library;

class ImageParser {
private:
    std::string path;
    TextureType type;

public:
    ImageParser(std::string _path, TextureType _type);

    [[nodiscard]] std::unique_ptr<const OldTexture> parse() const;
};
