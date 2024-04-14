#include <Math.hpp>

#include <optional>
#include <string>

std::optional<int> Math::optStoi(const std::string& str)
{
    if (str.empty()) {
        return std::nullopt;
    }

    return stoi(str);
}
