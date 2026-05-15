#include "MediaScanner.h"
#include <unordered_set>
#include <algorithm>

MediaScanner::MediaScanner(fs::path rootPath) :
    rootPath(std::move(rootPath))
{}

std::string MediaScanner::getFileCategory(const fs::path& filePath) const
{
    std::string fileExtension = filePath.extension().string();
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    static const std::unordered_set<std::string> audioExtension = {".mp3", ".wav", ".flac"};
    static const std::unordered_set<std::string> videoExtension = {".mp4", ".avi", ".mkv", ".mpg"};
    static const std::unordered_set<std::string> imageExtension = {".jpg", ".jpeg", ".png", ".gif"};

    if (audioExtension.count(fileExtension))
    {
        return "audio";
    }
    if (videoExtension.count(fileExtension))
    {
        return "video";
    }
    if (imageExtension.count(fileExtension))
    {
        return "images";
    }
    return "";
}

ScanResult MediaScanner::scan()
{
    ScanResult result;
    if (!fs::exists(rootPath) || !fs::is_directory(rootPath))
    {
        return result; 
    }

    for (const auto& entry : fs::recursive_directory_iterator(rootPath))
    {
        if (fs::is_regular_file(entry))
        {
            std::string category = getFileCategory(entry.path());
            if (!category.empty())
            {
                result[category].push_back(entry.path().filename().string());
            }
        }
    }
    return result;
}