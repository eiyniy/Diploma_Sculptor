#include <Texture.hpp>

#include <IBuilder.hpp>

#include <GL/glew.h>

#include <stdexcept>
#include <string>
#include <string_view>

Texture::Texture(ConstructorPasskey<Texture>&& passkey)
    : isBinded(false)
    , width(0)
    , height(0)
    , texture(0)
    , textureBlock(0)
    , dimensionality(0)
{
}

void Texture::init(
    const std::string_view _name,
    const int _textureBlock,
    const int _dimensionality)
{
    if (_name.length() == 0) {
        throw std::length_error("Texture name is too short.");
    }

    dimensionality = _dimensionality;
    textureBlock = _textureBlock;
    name = _name;

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
    glBindTexture(dimensionality, 0);

    isBinded = false;
}

int Texture::getTextureBlock() const { return textureBlock; }

std::string_view Texture::getName() const { return name; }
