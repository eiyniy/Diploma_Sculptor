#include <Math.hpp>

#include <SphericalCoordinate.hpp>
#include <Matrix.hpp>

#include <cmath>
#include <string>
#include <optional>

std::optional<int> Math::optStoi(const std::string &str)
{
    if (str.empty())
        return std::nullopt;
    else
        return stoi(str);
}

SphericalCoordinate Math::decartToSpherical(const Vector<4> &cv)
{
    const auto x = cv.cGetX();
    const auto y = cv.cGetZ();
    const auto z = cv.cGetY();

    static const auto toDegree = 180 / M_PI;

    const auto r = std::sqrt(
        std::pow(x, 2) +
        std::pow(y, 2) +
        std::pow(z, 2));

    double a = std::acos(z / r) * toDegree;
    double b = std::atan(y / x) * toDegree;

    if (x <= 0)
        b += 180;
    else if (x >= 0 && y < 0)
        b += 360;

    // NaN check
    if (a != a)
        a = 0.f;
    if (b != b)
        b = 0.f;

    return {r, a, b};
}

Vector<4> Math::sphericalToDecart(const SphericalCoordinate &sc)
{
    static const auto toRad = M_PI / 180;

    const auto radA = sc.a * toRad;
    const auto radB = sc.b * toRad;

    const auto sinA = std::sin(radA);

    const auto x = sc.r * sinA * std::cos(radB);
    const auto z = sc.r * sinA * std::sin(radB);
    const auto y = sc.r * std::cos(radA);

    return {x, y, z};
}
