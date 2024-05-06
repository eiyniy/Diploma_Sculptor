#include <Brush.hpp>

#include <stdexcept>
#include <string>

Brush::Brush(const float _radius, const float _strength)
    : radius(_radius)
    , strength(_strength)
{
    std::string error { "Can't create Brush. " };

    if (radius < 0) {
        throw std::logic_error(error + "Radius must be greater than 0.");
    }

    if (strength < 0) {
        throw std::logic_error(error + "Strength must be greater than 0.");
    }
}

float Brush::getRadius() const { return radius; }

float Brush::getStrength() const { return strength; }
