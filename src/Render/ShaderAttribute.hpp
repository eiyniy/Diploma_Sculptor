#pragma once

#include <GL/glew.h>

#include <string>
#include <string_view>

class ShaderAttribute {
private:
    std::string name;

    GLuint index;

    GLint elementsCount;

    GLenum type;

    unsigned sizeofElement;

    GLboolean shouldNormalize;

    bool isEnabled;

public:
    ShaderAttribute(
        std::string_view _name,
        GLuint _index,
        GLint _elementsCount,
        GLenum _type,
        unsigned _sizeofElement,
        GLboolean _shouldNormalize);

    void enable(GLsizei stride, GLvoid* offset);

    [[nodiscard]] std::string_view getName() const;

    [[nodiscard]] GLint getElementsCount() const;

    [[nodiscard]] unsigned getSizeofElement() const;
};
