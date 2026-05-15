#include "parseUtils.h"
#include "utils.h"

#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace fs = std::filesystem;

std::optional<AppConfig> parseArguments(int argc, char* argv[])
{
    AppConfig config;
    
    const char* homeDir = std::getenv("HOME");
    config.targetPath = homeDir ? homeDir : ".";
    config.configFilePath = (fs::path(config.targetPath) / ".media_files").string();

    for (size_t i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if ((arg == "--path" || arg == "-p") && (i + 1 < argc))
        {
            config.targetPath = argv[++i];
        }
        else if ((arg == "--interval" || arg == "-i") && (i + 1 < argc))
        {
            auto intervalSeconds = convertFromString<int>(argv[++i]);
            if (!intervalSeconds.has_value())
            {
                return std::nullopt;
            }
            config.intervalSeconds = intervalSeconds.value();
        }
        else if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: MediaScanner [options]\n"
                      << "Options:\n"
                      << "  -p, --path <path>      Path to scan (default: $HOME)\n"
                      << "  -i, --interval <sec>   Scan interval in seconds (default: 10)\n"
                      << "  -h, --help             Show this help\n";
            exit(0);
        }
    }
    return config;
}