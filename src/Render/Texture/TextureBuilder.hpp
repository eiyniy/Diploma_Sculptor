#pragma once

#include <IBuilder.hpp>
#include <Texture.hpp>

#include <string>
#include <string_view>

class TextureBuilder : public IBuilder<Texture> {
private:
    bool isLoaded;

    bool isInited;

    void reset() override;

    [[nodiscard]] bool isFinished() const override;

public:
    TextureBuilder();

    TextureBuilder(const TextureBuilder&) = delete;
    TextureBuilder(TextureBuilder&&) = delete;
    TextureBuilder& operator=(const TextureBuilder&) = delete;
    TextureBuilder& operator=(TextureBuilder&&) = delete;

    virtual ~TextureBuilder() = default;

    void init(std::string_view _name, int _textureBlock, int _dimensionality);

    void bind();

    void unbind();

    void setWrapping(int axis, int method);

    void setFiltering(int filter, int method);

    void setDefaults();

    void load(const std::string& path);
};