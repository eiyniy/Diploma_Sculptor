#include <BaseTextParser.hpp>

#include <cctype>
#include <filesystem>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

// TODO Replace all (const std::string &) with (std::string_view)

BaseTextParser::BaseTextParser(std::string _pathToFile)
    : pathToFile(std::move(_pathToFile))
{
    std::error_code ec;
    if (!std::filesystem::exists(pathToFile, ec)) {
        throw std::logic_error("Could not open file");
    }
}

std::vector<std::string> BaseTextParser::splitByLines(const std::string& string)
{
    auto result = std::vector<std::string> {};
    auto ss = std::stringstream { string };

    for (std::string line; std::getline(ss, line, '\n');) {
        result.emplace_back(line);
    }

    return result;
}

std::optional<std::string> BaseTextParser::getNextPart(
    std::string::const_iterator* iter,
    std::string::const_iterator iterEnd,
    char divider,
    bool allowEmpty)
{
    if (*iter >= iterEnd) {
        return std::nullopt;
    }

    auto iterSecond = *iter;

    while (iterSecond < iterEnd && *iterSecond != divider) {
        ++iterSecond;
    }

    auto result = std::string(*iter, iterSecond);

    *iter = iterSecond;

    if (allowEmpty) {
        do { // NOLINT
            ++(*iter);
        } while (*iter < iterEnd && **iter != divider && **iter != '-'
                 && (isdigit(**iter) == 0));
    } else {
        while (*iter < iterEnd && (**iter == divider || **iter == '\r')) {
            ++(*iter);
        }
    }

    return result;
}

std::string BaseTextParser::readFile()
{
    readStream.open(pathToFile, std::ios::binary);
    if (!readStream.is_open()) {
        throw std::logic_error("Could not open file");
    }

    const auto size = std::filesystem::file_size(pathToFile);
    auto buffer = std::string(size, 0);

    readStream.read(buffer.data(), static_cast<std::streamsize>(size));
    readStream.close();

    return buffer;
}

std::string BaseTextParser::getPathToFile() const { return pathToFile; }
