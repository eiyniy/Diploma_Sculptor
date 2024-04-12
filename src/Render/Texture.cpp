#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <Texture.hpp>

#include <GL/glew.h>

#include <SOIL.h>

#include <stdexcept>
#include <string>

Texture::Texture(const int _textureBlock, const int _dimensionality)
    : dimensionality(_dimensionality)
    , textureBlock(_textureBlock)
    , isBinded(false)
    , image(nullptr)
    , width(0)
    , height(0)
    , texture(0)
{
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

    image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(
        dimensionality,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
}

int Texture::getTextureBlock() const { return textureBlock; }
