#pragma once

#include <GL/glew.h>

#include <string>
#include <string_view>

class Texture {
private:
    GLuint texture;

    int textureBlock;
    int dimensionality;

    bool isBinded;

    int width, height;

    std::string name;

    void throwIfNotBinded(const std::string& message) const;

public:
    Texture(std::string_view _name, int _textureBlock, int _dimensionality);

    void bind();
    void unbind();

    void setWrapping(int axis, int method);

    void setFiltering(int filter, int method);

    void setDefaults();

    void load(const std::string& path);

    [[nodiscard]] int getTextureBlock() const;

    [[nodiscard]] std::string_view getName() const;
};
