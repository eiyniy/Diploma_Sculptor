#pragma once

#include <GL/glew.h>

#include <string>
#include <string_view>

template <class T> class ConstructorPasskey;

class Texture {

    friend class TextureBuilder;

private:
    GLuint texture;

    int textureBlock;

    int dimensionality;

    bool isBinded;

    int width;
    int height;

    std::string name;

    void init(std::string_view _name, int _textureBlock, int _dimensionality);

    void throwIfNotBinded(const std::string& message) const;

public:
    Texture(ConstructorPasskey<Texture>&& passkey);

    void bind();

    void unbind();

    [[nodiscard]] int getTextureBlock() const;

    [[nodiscard]] std::string_view getName() const;
};
