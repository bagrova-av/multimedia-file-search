#include <iostream>
#include <thread>
#include <chrono>

#include "./MediaScanner/MediaScanner.h"
#include "./common/parseUtils.h"
#include "./common/utils.h"

int main(int argc, char** argv)
{
    std::optional<AppConfig> config = parseArguments(argc, argv);
    if (!config.has_value())
    {
        return 1;
    }

    MediaScanner mediaScanner(config.value().targetPath);

    std::cout << "Starting media scanner on: " << config.value().targetPath << '\n';
    std::cout << "Interval: " << config.value().intervalSeconds << "sec" << '\n';

    while (true)
    {
        std::cout << "Scanning..." << '\n';

        auto results = mediaScanner.scan();
        std::string jsonStr = mediaScanner.getJsonResult(results);

        saveJsonToFile(config.value().configFilePath, jsonStr);

        std::this_thread::sleep_for(std::chrono::seconds(config.value().intervalSeconds));
    }
}