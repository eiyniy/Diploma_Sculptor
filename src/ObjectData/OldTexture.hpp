#pragma once

#include <vector_float4.hpp>

#include <vector>

class OldTexture {
private:
    int width;
    int height;
    std::vector<glm::vec4> data;

public:
    OldTexture(int _width, int _height, const std::vector<glm::vec4>& _data);

    [[nodiscard]] int cGetWidth() const;
    [[nodiscard]] int cGetHeight() const;
    [[nodiscard]] const std::vector<glm::vec4>& cGetData() const;
};
