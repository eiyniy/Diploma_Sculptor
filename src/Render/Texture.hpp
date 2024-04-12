#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
private:
    GLuint texture;

    int textureBlock;
    int dimensionality;

    bool isBinded;

    unsigned char* image;
    int width, height;

    void throwIfNotBinded(const std::string& message) const;

public:
    Texture(int _textureBlock, int _dimensionality);

    void bind();
    void unbind();

    void setWrapping(int axis, int method);

    void setFiltering(int filter, int method);

    void setDefaults();

    void load(const std::string& path);

    [[nodiscard]] int getTextureBlock() const;
};
