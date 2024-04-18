#include <ShaderAttribute.hpp>
#include <stdexcept>

ShaderAttribute::ShaderAttribute(
    const std::string_view _name,
    const GLuint _index,
    const GLint _elementsCount,
    const GLenum _type,
    unsigned _sizeofElement,
    const GLboolean _shouldNormalize)
    : name(_name)
    , index(_index)
    , elementsCount(_elementsCount)
    , type(_type)
    , sizeofElement(_sizeofElement)
    , shouldNormalize(_shouldNormalize)
    , isEnabled(false)
{
}

void ShaderAttribute::enable(const GLsizei stride, GLvoid* const offset)
{
    if (isEnabled) {
        throw std::logic_error(
            "Can't enable shader attribute. It's already enabled");
    }

    glVertexAttribPointer(
        index, elementsCount, type, shouldNormalize, stride, offset);
    glEnableVertexAttribArray(index);

    isEnabled = true;
}

std::string_view ShaderAttribute::getName() const { return name; }

GLint ShaderAttribute::getElementsCount() const { return elementsCount; }

unsigned ShaderAttribute::getSizeofElement() const { return sizeofElement; }
