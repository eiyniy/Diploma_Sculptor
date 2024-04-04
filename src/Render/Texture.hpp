#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include <string>

// TODO: TextureLoader
class Texture
{
private:
    GLuint texture;

    const int textureBlock;
    const int dimensionality;

    bool isBinded;

    unsigned char *image;
    int width, height;

    void throwIfNotBinded(const std::string &message);

public:
    Texture(
        const int _textureBlock,
        const int _dimensionality);

    void bind();
    void unbind();

    void setWrapping(
        const int axis,
        const int method);

    void setFiltering(
        const int filter,
        const int method);

    void setDefaults();

    void load(const std::string &path);
};
