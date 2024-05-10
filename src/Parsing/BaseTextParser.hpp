#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

class BaseTextParser {
public:
    static std::vector<std::string_view>
    splitByLines(const std::string& string);

    // TODO: Remove
    static std::optional<std::string> getNextPart(
        std::string::const_iterator* iter,
        std::string::const_iterator iterEnd,
        char divider,
        bool allowEmpty = false);

    std::string readFile(const std::string& pathToFile);

private:
    std::ifstream readStream;
};
