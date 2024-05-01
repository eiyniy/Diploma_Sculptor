#pragma once

#include <climits>
#include <cstddef>
#include <functional>
#include <limits>

class Hasher {
private:
    static constexpr std::size_t zeroOnePattern = 0x5555555555555555ULL;
    static constexpr std::size_t randomSeed = 17316035218449499591ULL;
    static constexpr int distributeShift = sizeof(size_t) * CHAR_BIT / 2;

    template <typename T> static T xorShift(const T& n, int i);
    std::size_t static distribute(const std::size_t& n);

public:
    template <class T>
    static std::size_t hashCombine(std::size_t& seed, const T& v);
};

template <typename T> T Hasher::xorShift(const T& n, const int i)
{
    return n ^ (n >> i);
}

// a hash function with another name as to not confuse with std::hash
std::size_t Hasher::distribute(const std::size_t& n)
{
    return randomSeed
        * xorShift(
               zeroOnePattern * xorShift(n, distributeShift), distributeShift);
}

// call this function with the old seed and the new key to be hashed and
// combined into the new seed value, respectively the final hash
template <class T>
inline std::size_t Hasher::hashCombine(std::size_t& seed, const T& v)
{
    return std::rotl(seed, std::numeric_limits<std::size_t>::digits / 3)
        ^ distribute(std::hash<T> {}(v));
}
