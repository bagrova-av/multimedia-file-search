#include <iostream>

#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

#include "./MediaScanner/MediaScanner.h"
#include "./MediaServer/MediaServer.h"
#include "./common/parseUtils.h"
#include "./common/utils.h"

std::atomic<bool> keepRunning(true);

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        keepRunning = false;
    }
}


int main(int argc, char** argv)
{
    std::signal(SIGINT, signalHandler);

    std::optional<AppConfig> config = parseArguments(argc, argv);
    if (!config.has_value())
    {
        return 1;
    }

    MediaScanner mediaScanner(config.value().targetPath);
    MediaServer mediaServer("0.0.0.0", 1234);

    std::thread mediaServerThread([&mediaServer]()
                            {
                                mediaServer.run();
                            }
    );

    std::cout << "Starting media scanner on: " << config.value().targetPath << '\n';
    std::cout << "Interval: " << config.value().intervalSeconds << "sec" << '\n';

     std::cout << "Press Ctrl+C to stop" << '\n';
    
    while (keepRunning)
    {
        std::cout << "Scanning..." << '\n';

        auto results = mediaScanner.scan();
        std::string jsonStr = mediaScanner.getJsonResult(results);

        saveJsonToFile(config.value().configFilePath, jsonStr);
        mediaServer.updateData(std::move(jsonStr));

        for (int i = 0; i < config.value().intervalSeconds && keepRunning; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "\nShutting down..." << '\n';
    mediaServer.stop();

    if (mediaServerThread.joinable())
    {
        mediaServerThread.join();
    }

    std::cout << "Server stopped" << '\n';
    return 0;
}