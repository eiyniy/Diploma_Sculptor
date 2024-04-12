#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

class BaseTextParser {
public:
    BaseTextParser(std::string _pathToFile);

    static std::vector<std::string> splitByLines(const std::string& string);

    static std::optional<std::string> getNextPart(
        std::string::const_iterator* iter,
        std::string::const_iterator iterEnd,
        char divider,
        bool allowEmpty = false);

    std::string readFile();

protected:
    std::string getPathToFile() const;

private:
    std::string pathToFile;
    std::ifstream readStream;
};

inline std::string BaseTextParser::getPathToFile() const { return pathToFile; }
