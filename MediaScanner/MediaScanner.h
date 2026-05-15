#pragma once
#ifndef MEDIA_SCANNER_H
#define MEDIA_SCANNER_H

#include <filesystem>

#include <vector>
#include <string>
#include <map>

namespace fs = std::filesystem;

using FileList = std::vector<std::string>;
using ScanResult = std::map<std::string, FileList>;

class MediaScanner
{
public:
    explicit MediaScanner(fs::path rootPath);

    ScanResult scan();

private:
    fs::path rootPath;

private:
    std::string getFileCategory(const fs::path& filePath) const;
};

#endif