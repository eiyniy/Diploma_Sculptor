#include <SpotLight.hpp>

#include <Matrix.hpp>

SpotLight::SpotLight(Vector<4>& _position)
    : position(_position)
{
}

Vector<4> SpotLight::getLightDirection(const Vector<4>& target) const
{
    auto tDir = target - position;
    tDir.normalize();
    return tDir;
}
