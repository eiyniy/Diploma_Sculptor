#pragma once

#include <SphericalCoordinate.hpp>
#include <Matrix.hpp>

#include <optional>
#include <string>

class Math
{
public:
    Math() = delete;

    static std::optional<int> optStoi(const std::string &str);
    static SphericalCoordinate decartToSpherical(const Vector<4> &cv);
    static Vector<4> sphericalToDecart(const SphericalCoordinate &sc);
};
