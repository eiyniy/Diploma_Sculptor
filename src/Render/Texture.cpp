#include <Texture.hpp>

#include <GL/glew.h>

#include <SOIL.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

Texture::Texture(
    const std::string_view _name,
    const int _textureBlock,
    const int _dimensionality)
    : dimensionality(_dimensionality)
    , textureBlock(_textureBlock)
    , isBinded(false)
    , width(0)
    , height(0)
    , texture(0)
    , name(_name)
{
    if (name.length() == 0) {
        throw std::length_error("Texture name is too short.");
    }

    glGenTextures(1, &texture);
}

void Texture::throwIfNotBinded(const std::string& message = std::string()) const
{
    if (!isBinded) {
        throw std::logic_error("Texture isn't binded. " + message);
    }
}

void Texture::bind()
{
    glActiveTexture(textureBlock);
    glBindTexture(dimensionality, texture);

    isBinded = true;
}

void Texture::unbind()
{
    throwIfNotBinded("unbind");
    glBindTexture(dimensionality, 0);

    isBinded = false;
}

void Texture::setWrapping(const int axis, const int method)
{
    throwIfNotBinded("setWrapping");
    glTexParameteri(dimensionality, axis, method);
}

void Texture::setFiltering(const int filter, const int method)
{
    throwIfNotBinded("setFiltering");
    glTexParameteri(dimensionality, filter, method);
}

void Texture::setDefaults()
{
    throwIfNotBinded("setDefaults");

    glTexParameteri(dimensionality, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(dimensionality, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(dimensionality, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(dimensionality, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::load(const std::string& path)
{
    throwIfNotBinded("load");

    std::unique_ptr<unsigned char> image { SOIL_load_image(
        path.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB) };

    glTexImage2D(
        dimensionality,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image.get());
    glGenerateMipmap(GL_TEXTURE_2D);
}

int Texture::getTextureBlock() const { return textureBlock; }

std::string_view Texture::getName() const { return name; }
