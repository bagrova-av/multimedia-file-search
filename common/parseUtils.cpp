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
        if ((arg == "--path" || arg == "-p"))
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: Missing value for " + arg << '\n';
                return std::nullopt;
            }

            config.targetPath = argv[++i];
        }
        else if ((arg == "--interval" || arg == "-i"))
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: Missing value for " + arg << '\n';
                return std::nullopt;
            }

            auto intervalSeconds = convertFromString<int>(argv[++i]);
            if (!intervalSeconds.has_value())
            {
                std::cerr << "Error: frequency with which the catalog is monitored must be an integer number" << '\n';
                return std::nullopt;
            }
            config.intervalSeconds = intervalSeconds.value();
        }
        else if (arg == "--web" || arg == "-w")
        {
            config.mode = OutputMode::HTTP;
        }
        else if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: MediaScanner [options]\n"
                      << "Options:\n"
                      << "  -p, --path <path>      Path to scan (default: $HOME)\n"
                      << "  -i, --interval <sec>   Scan interval in seconds (default: 10)\n"
                      << "  -w, --web              Get the result via HTTP (default: in file)\n"
                      << "  -h, --help             Show this help\n";
            exit(0);
        }
        else
        {
            std::cerr << "Error: an unsupported flag has been entered" << '\n';
            std::cout << "Usage: MediaScanner [options]\n"
                      << "Options:\n"
                      << "  -p, --path <path>      Path to scan (default: $HOME)\n"
                      << "  -i, --interval <sec>   Scan interval in seconds (default: 10)\n"
                      << "  -w, --web              Get the result via HTTP (default: in file)\n"
                      << "  -h, --help             Show this help\n";
            exit(1);
        }
    }
    return config;
}