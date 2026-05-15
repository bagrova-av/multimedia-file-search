#pragma once
#ifndef MEDIA_SCANNER_H
#define MEDIA_SCANNER_H

#include <filesystem>

#include <optional>
#include <vector>
#include <string>
#include <map>

namespace fs = std::filesystem;

enum class MediaType
{
    AUDIO,
    VIDEO,
    IMAGE
};

using FileList = std::vector<std::string>;
using ScanResult = std::map<MediaType, FileList>;

class MediaScanner
{
public:
    explicit MediaScanner(fs::path rootPath);

    ScanResult scan();
    std::string getJsonResult(const ScanResult& result) const;

private:
    fs::path rootPath;

private:
    std::optional<MediaType> getFileCategory(const fs::path& filePath) const;
    std::string mediaTypeToString(MediaType type) const;
};

#endif