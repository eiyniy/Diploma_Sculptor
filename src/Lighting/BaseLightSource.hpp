#pragma once

#include <Matrix.hpp>

class BaseLightSource {
public:
    BaseLightSource() = default;

    BaseLightSource(const BaseLightSource&) = default;
    BaseLightSource(BaseLightSource&&) = delete;
    BaseLightSource& operator=(const BaseLightSource&) = default;
    BaseLightSource& operator=(BaseLightSource&&) = delete;

    virtual ~BaseLightSource() = default;

    [[nodiscard]] virtual Vector<4>
    getLightDirection(const Vector<4>& target) const = 0;
};
