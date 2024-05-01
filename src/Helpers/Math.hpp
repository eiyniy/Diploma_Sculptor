#pragma once

#include <Concepts.hpp>

#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <string>

class Math {
public:
    static constexpr std::size_t defaultULP = 1U;
    static constexpr float fastEpsilon = 0.000001F;

    Math() = delete;

    static std::optional<int> optStoi(const std::string& str);

    template <class T>
        requires NotInteger<T>
    static T getEpsilon(T x, T y, std::size_t n = defaultULP);
};

template <class T>
    requires NotInteger<T>
T Math::getEpsilon(T x, T y, std::size_t n)
{
    // Since `epsilon()` is the gap size (ULP, unit in the last place)
    // of floating-point numbers in interval [1, 2), we can scale it to
    // the gap size in interval [2^e, 2^{e+1}), where `e` is the exponent
    // of `x` and `y`.

    // If `x` and `y` have different gap sizes (which means they have
    // different exponents), we take the smaller one. Taking the bigger
    // one is also reasonable, I guess.
    const T m = std::max(std::fabs(x), std::fabs(y));

    // Subnormal numbers have fixed exponent, which is `min_exponent - 1`.
    const int exp = m < std::numeric_limits<T>::min()
        ? std::numeric_limits<T>::min_exponent - 1
        : std::ilogb(m);

    // We consider `x` and `y` equal if the difference between them is
    // within `n` ULPs.
    return n * std::ldexp(std::numeric_limits<T>::epsilon(), exp);
}
