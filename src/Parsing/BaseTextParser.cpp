#include <BaseTextParser.hpp>

#include <cctype>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

std::vector<std::string_view>
BaseTextParser::splitByLines(const std::string& string)
{
    std::vector<std::string_view> lines;
    std::size_t start = 0;
    std::size_t end = 0;

    while (end != std::string_view::npos) {
        end = string.find_first_of('\n', start);
        std::size_t len = (end == std::string_view::npos)
            ? string.length() - start
            : end - start;
        lines.emplace_back(string.data() + start, len);
        start = end + 1;
    }

    return lines;
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

std::string BaseTextParser::readFile(const std::string& pathToFile)
{
    std::error_code ec;
    if (!std::filesystem::exists(pathToFile, ec)) {
        throw std::logic_error("Could not open file");
    }

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
