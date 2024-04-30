#pragma once

#include <cmath>
#include <cstddef>
#include <optional>
#include <string>

class Math {
public:
    static constexpr size_t defaultULP = 5U;

    Math() = delete;

    static std::optional<int> optStoi(const std::string& str);

    template <class T>
    static std::enable_if_t<not std::numeric_limits<T>::is_integer, bool>
    equalWithinUlps(T x, T y, std::size_t n = defaultULP);
};

// Replace with float getEpsilon(T x, T y, size_t n)
template <class T>
static std::enable_if_t<not std::numeric_limits<T>::is_integer, bool>
equalWithinUlps(T x, T y, std::size_t n)
{
    // Since `epsilon()` is the gap size (ULP, unit in the last place)
    // of floating-point numbers in interval [1, 2), we can scale it to
    // the gap size in interval [2^e, 2^{e+1}), where `e` is the exponent
    // of `x` and `y`.

    // If `x` and `y` have different gap sizes (which means they have
    // different exponents), we take the smaller one. Taking the bigger
    // one is also reasonable, I guess.
    const T m = std::min(std::fabs(x), std::fabs(y));

    // Subnormal numbers have fixed exponent, which is `min_exponent - 1`.
    const int exp = m < std::numeric_limits<T>::min()
        ? std::numeric_limits<T>::min_exponent - 1
        : std::ilogb(m);

    // We consider `x` and `y` equal if the difference between them is
    // within `n` ULPs.
    return std::fabs(x - y)
        <= n * std::ldexp(std::numeric_limits<T>::epsilon(), exp);
}
