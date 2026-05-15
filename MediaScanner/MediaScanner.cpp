#include "MediaScanner.h"

#include <iostream>

#include <unordered_set>
#include <algorithm>

#include <nlohmann/json.hpp>

MediaScanner::MediaScanner(fs::path rootPath) :
    rootPath(std::move(rootPath))
{}

std::optional<MediaType> MediaScanner::getFileCategory(const fs::path& filePath) const
{
    std::string fileExtension = filePath.extension().string();
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    static const std::unordered_set<std::string> audioExtension = {".mp3", ".wav", ".flac"};
    static const std::unordered_set<std::string> videoExtension = {".mp4", ".avi", ".mkv", ".mpg"};
    static const std::unordered_set<std::string> imageExtension = {".jpg", ".jpeg", ".png", ".gif"};

    if (audioExtension.count(fileExtension))
    {
        return MediaType::AUDIO;
    }
    if (videoExtension.count(fileExtension))
    {
        return MediaType::VIDEO;
    }
    if (imageExtension.count(fileExtension))
    {
        return MediaType::IMAGE;
    }
    return std::nullopt;
}

std::string MediaScanner::mediaTypeToString(MediaType type) const
{
    switch (type)
    {
        case MediaType::AUDIO:
        {
            return "audio";
        }
        case MediaType::VIDEO:
        {
            return "video";
        }
        case MediaType::IMAGE:
        {
            return "images";
        }
        default:
        {
            return "unknown";
        }
    }
}

ScanResult MediaScanner::scan()
{
    ScanResult result;
    std::error_code errorCode;

    if (!fs::exists(rootPath, errorCode))
    {
        if (errorCode)
        {
            std::cerr << "Error reading path: " << rootPath << " - " << errorCode.message() << '\n';
        }
        else
        {
            std::cerr << "Path does not exist: " << rootPath << '\n';
        }
        return result;
    }
    errorCode.clear();
    if (!fs::is_directory(rootPath, errorCode))
    {
        std::cerr << "Path is not a directory: " << rootPath << '\n';
        return result;
    }

    fs::directory_options options = fs::directory_options::skip_permission_denied;

    fs::recursive_directory_iterator it(rootPath, options, errorCode);
    fs::recursive_directory_iterator end;
    while (it != end)
    {
        if (errorCode)
        {
            std::cerr << "Warning: skipping path due to error: " << errorCode.message() << '\n';
            errorCode.clear();
            it.increment(errorCode);
            continue;
        }
        errorCode.clear();

        const auto& entry = *it;
        if (fs::is_regular_file(entry, errorCode))
        {
            std::optional<MediaType> category = getFileCategory(entry.path());
            if (category.has_value())
            {
                result[category.value()].push_back(entry.path().filename().string());
            }
        }
        it.increment(errorCode);
    }
    return result;
}

std::string MediaScanner::getJsonResult(const ScanResult& result) const
{
    nlohmann::json j;

    j["audio"] = nlohmann::json::array();
    j["video"] = nlohmann::json::array();
    j["images"] = nlohmann::json::array();

    for (const auto& [type, files] : result)
    {
        std::string key = mediaTypeToString(type);
        if (key != "unknown")
        {
            j[key] = files;
        }
    }

    return j.dump(4);
}