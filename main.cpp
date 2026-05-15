#include <iostream>

#include <cstdlib>
#include <thread>

#include "./MediaScanner/MediaScanner.h"

int main(int argc, char** argv)
{
    const char* homeDir = std::getenv("HOME");
    if (!homeDir)
    {
        std::cerr << "Could not find HOME directory" << '\n';
        return 1;
    }

    std::string targetPath = homeDir;
    size_t intervalSeconds = 5;

    if (argc > 1)
    {
        targetPath = argv[1];
    }
    if (argc > 2)
    {
        intervalSeconds = std::stoi(argv[2]);
    }

    MediaScanner mediaScanner(targetPath);

    std::cout << "Starting media scanner on: " << targetPath << '\n';
    std::cout << "Interval: " << intervalSeconds << "sec" << '\n';

    while (true)
    {
        std::cout << "Scanning..." << '\n';

        auto results = mediaScanner.scan();
        std::string jsonStr = mediaScanner.getJsonResult(results);
        
        std::cout << jsonStr << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}