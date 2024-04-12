#include <MtlParser.hpp>

#include <BaseTextParser.hpp>
#include <Enums.hpp>
#include <ImageParser.hpp>
#include <Material.hpp>
#include <Matrix.hpp>
#include <OldTexture.hpp>

#include <array>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

const auto chanelMaxValue = 255.F;

MtlParser::MtlParser(const std::string& _pathToMtl)
    : BaseTextParser(_pathToMtl)
{
    resetMaterial();

    materials = std::make_unique<
        std::map<std::string, std::shared_ptr<const Material>>>();
}

std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>>
MtlParser::parse()
{
    const auto fileContent = readFile();
    const auto lines = splitByLines(fileContent);

    for (const auto& line : lines) {
        parseEntry(line);
    }

    if (!name.empty()) {
        (*materials)[name] = std::make_shared<const Material>(
            name,
            ambient,
            diffuse,
            specular,
            specularExp,
            std::move(diffuseMap),
            std::move(emissiveMap),
            std::move(normalMap),
            std::move(mraoMap));
    }

    resetMaterial();

    return std::move(materials);
}

void MtlParser::parseEntry(const std::string& line)
{
    const auto type = getEntryType(line);
    if (!type.has_value()) {
        return;
    }

    auto iter = line.cbegin();
    const auto iterEnd = line.cend();

    MtlParser::getNextPart(&iter, iterEnd, ' ');

    switch (*type) {
    case MtlEntryType::NewMaterial: {
        if (!name.empty()) {
            (*materials)[name] = std::make_shared<const Material>(
                name,
                ambient,
                diffuse,
                specular,
                specularExp,
                std::move(diffuseMap),
                std::move(emissiveMap),
                std::move(normalMap),
                std::move(mraoMap));
        }

        resetMaterial();

        const auto optName = MtlParser::getNextPart(&iter, iterEnd, ' ');

        if (!optName) {
            throw std::logic_error("Can't parse material name");
        }

        name = *optName;
        break;
    }
    case MtlEntryType::Ambient:
        ambient = parseCoeff(line);
        break;
    case MtlEntryType::Diffuse:
        diffuse = parseCoeff(line);
        break;
    case MtlEntryType::Specular:
        specular = parseCoeff(line);
        break;
    case MtlEntryType::SpecularExp: {
        const auto optSpecularExpString
            = MtlParser::getNextPart(&iter, iterEnd, ' ');

        if (!optSpecularExpString) {
            throw std::logic_error("Can't parse specular exponent");
        }

        specularExp = std::stod(*optSpecularExpString);
        break;
    }
    case MtlEntryType::DiffuseMap:
        diffuseMap = parseTexture(line, getPathToFile());
        break;
    case MtlEntryType::EmissiveMap:
        emissiveMap = parseTexture(line, getPathToFile());
        break;
    case MtlEntryType::NormalMap:
        normalMap = parseTexture(line, getPathToFile());
        break;
    case MtlEntryType::MRAOMap:
        mraoMap = parseTexture(line, getPathToFile());
        break;
    default:
        break;
    }
}

std::optional<MtlEntryType> MtlParser::getEntryType(const std::string& line)
{
    auto iter = line.begin();

    auto type = getNextPart(&iter, line.end(), ' ');

    if (!type) {
        return std::nullopt;
    }

    if (type == "newmtl") {
        return MtlEntryType::NewMaterial;
    }
    if (type == "Ka") {
        return MtlEntryType::Ambient;
    }
    if (type == "Kd") {
        return MtlEntryType::Diffuse;
    }
    if (type == "Ks") {
        return MtlEntryType::Specular;
    }
    if (type == "Ns") {
        return MtlEntryType::SpecularExp;
        // else if (type == "map_Ka")
        // return MtlEntryType::AmbientMap;
    }
    if (type == "map_Kd") {
        return MtlEntryType::DiffuseMap;
    }
    if (type == "map_Ke") {
        return MtlEntryType::EmissiveMap;
    }
    if (type == "norm") {
        return MtlEntryType::NormalMap;
    }
    if (type == "map_MRAO") {
        return MtlEntryType::MRAOMap;
    }
    return std::nullopt;
}

void MtlParser::resetMaterial()
{
    name.clear();
    ambient = std::nullopt;
    diffuse = std::nullopt;
    specular = std::nullopt;
    specularExp = std::nullopt;
    diffuseMap = nullptr;
    emissiveMap = nullptr;
    normalMap = nullptr;
    mraoMap = nullptr;
}

Vector<4> MtlParser::parseCoeff(const std::string& line)
{
    const auto entryType = MtlParser::getEntryType(line);
    if (entryType != MtlEntryType::Ambient && entryType != MtlEntryType::Diffuse
        && entryType != MtlEntryType::Specular) {
        throw std::logic_error("Can't parse material coefficent");
    }

    std::optional<std::string> strPart;
    auto accumulator = std::array<double, 3>();

    auto iter = line.cbegin();
    const auto iterEnd = line.cend();

    MtlParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    for (; (strPart = MtlParser::getNextPart(&iter, line.end(), ' ')); ++i) {
        accumulator.at(i) = std::stod(*strPart) * chanelMaxValue;
    }

    // if (i != 3)
    // throw std::logic_error("MtlParser. Can't parse value.");

    return { accumulator[0], accumulator[1], accumulator[2] };
}

std::unique_ptr<const OldTexture> MtlParser::parseTexture(
    const std::string& line,
    const std::string& pathToFile,
    std::optional<MtlEntryType> optType)
{
    if (!optType) {
        optType = MtlParser::getEntryType(line);
        if (!optType) {
            throw std::logic_error(
                "Can't parse material texture. Can't get entry type");
        }
    }

    const auto entryType = *optType;
    if (entryType != MtlEntryType::DiffuseMap
        && entryType != MtlEntryType::EmissiveMap
        && entryType != MtlEntryType::NormalMap
        && entryType != MtlEntryType::MRAOMap) {
        throw std::logic_error(
            "Can't parse material texture. Invalid entry type");
    }

    auto iter = line.cbegin();
    const auto iterEnd = line.cend();

    MtlParser::getNextPart(&iter, iterEnd, ' ');

    const auto optTexturePath = MtlParser::getNextPart(&iter, iterEnd, ' ');

    if (!optTexturePath) {
        throw std::logic_error(
            "Can't parse material texture. Invalid texture path");
    }

    TextureType textureType {};

    switch (entryType) {
    case MtlEntryType::DiffuseMap:
        textureType = TextureType::Diffuse;
        break;
    case MtlEntryType::EmissiveMap:
        textureType = TextureType::Emissive;
        break;
    case MtlEntryType::NormalMap:
        textureType = TextureType::Normal;
        break;
    case MtlEntryType::MRAOMap:
        textureType = TextureType::MRAO;
        break;
    default:
        throw std::logic_error(
            "Can't parse material texture. Invalid entry type");
    }

    const auto texturePath
        = pathToFile.substr(0, pathToFile.rfind('/') + 1) + *optTexturePath;

    const ImageParser parser { texturePath, textureType };
    return parser.parse();
}
