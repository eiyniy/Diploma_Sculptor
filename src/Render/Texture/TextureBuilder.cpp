#include <TextureBuilder.hpp>

#include <IBuilder.hpp>
#include <Texture.hpp>

#include <GL/glew.h>

#include <SOIL.h>

#include <memory>

TextureBuilder::TextureBuilder()
    : isInited(false)
    , isLoaded(false)
{
}

void TextureBuilder::reset()
{
    IBuilder<Texture>::reset();

    isLoaded = false;
    isInited = false;
}

void TextureBuilder::init(
    const std::string_view _name,
    const int _textureBlock,
    const int _dimensionality)
{
    instance->init(_name, _textureBlock, _dimensionality);
    isInited = true;
}

void TextureBuilder::bind() { instance->bind(); }

void TextureBuilder::unbind() { instance->unbind(); }

void TextureBuilder::setWrapping(int axis, int method)
{
    instance->throwIfNotBinded("setWrapping");

    glTexParameteri(instance->dimensionality, axis, method);
}

void TextureBuilder::setFiltering(int filter, int method)
{
    instance->throwIfNotBinded("setFiltering");

    glTexParameteri(instance->dimensionality, filter, method);
}

void TextureBuilder::setDefaults()
{
    instance->throwIfNotBinded("setDefaults");

    glTexParameteri(instance->dimensionality, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(instance->dimensionality, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(instance->dimensionality, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(instance->dimensionality, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureBuilder::load(const std::string& path)
{
    instance->throwIfNotBinded("load");

    std::unique_ptr<unsigned char> image { SOIL_load_image(
        path.c_str(),
        &instance->width,
        &instance->height,
        nullptr,
        SOIL_LOAD_RGB) };

    glTexImage2D(
        instance->dimensionality,
        0,
        GL_RGB,
        instance->width,
        instance->height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    isLoaded = true;
}

bool TextureBuilder::isFinished() const
{
    return IBuilder<Texture>::isFinished() && isLoaded && isInited
        && !instance->isBinded;
}
