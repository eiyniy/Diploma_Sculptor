#include <ImageParser.hpp>

#include <Enums.hpp>
#include <Globals.hpp>
#include <Matrix.hpp>
#include <OldTexture.hpp>

#include <CImg.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

const auto chanelMaxValue = 255.F;

ImageParser::ImageParser(std::string _path, const TextureType _type)
    : path(std::move(_path))
    , type(_type)
{
}

std::unique_ptr<const OldTexture> ImageParser::parse() const
{
    const cimg::CImg<char> image = cimg::CImg<>(path.c_str());

    int width = image.width();
    int height = image.height();

    std::vector<Vector<4>> data { (unsigned long long)width * height };
    // image.display();

#pragma omp parallel for if (!_IS_DEBUG)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const auto r = image(j, i, 0);
            const auto g = image(j, i, 1);
            const auto b = image(j, i, 2);

            if (type == TextureType::Diffuse || type == TextureType::Emissive) {
                data.at(j + width * i) = Vector<4>(r, g, b, 0);
            } else if (type == TextureType::Normal) {
                data.at(j + width * i) = Vector<4>(
                    static_cast<float>(r) / chanelMaxValue * 2 - 1,
                    static_cast<float>(g) / chanelMaxValue * 2 - 1,
                    static_cast<float>(b) / chanelMaxValue * 2 - 1,
                    0);
                data.at(j + width * i).normalize();
            } else if (type == TextureType::MRAO) {
                data.at(j + width * i) = Vector<4>(
                    static_cast<float>(r) / chanelMaxValue,
                    static_cast<float>(g) / chanelMaxValue,
                    static_cast<float>(b) / chanelMaxValue,
                    0);
                data.at(j + width * i).normalize();
            }
        }
    }

    return std::make_unique<const OldTexture>(width, height, data);
}
