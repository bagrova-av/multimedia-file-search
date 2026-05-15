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
    std::unique_ptr<MediaServer> mediaServer;
    std::thread mediaServerThread;

    if (config.value().mode == OutputMode::HTTP)
    {
        mediaServer = std::make_unique<MediaServer>("0.0.0.0", 1234);
        mediaServerThread = std::thread([&mediaServer]()
                                        {
                                            mediaServer->run();
                                        }
        );
        std::cout << "Mode: HTTP Server (http://localhost:1234/media_files)" << '\n';
    }
    else
    {
        std::cout << "Mode: file System (.media_files)" << '\n';
    }
    std::cout << "Interval: " << config.value().intervalSeconds << "sec" << '\n';
    std::cout << "Press Ctrl+C to stop" << '\n';
    
    std::cout << "Scanning..." << '\n';
    while (keepRunning)
    {
        auto results = mediaScanner.scan();
        std::string jsonStr = mediaScanner.getJsonResult(results);

        if (config.value().mode == OutputMode::HTTP)
        {
            mediaServer->updateData(std::move(jsonStr));
        }
        else
        {
            saveJsonToFile(config.value().configFilePath, jsonStr);
        }

        for (int i = 0; i < config.value().intervalSeconds && keepRunning; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "\nShutting down..." << '\n';
    if (mediaServer)
    {
        mediaServer->stop();
        if (mediaServerThread.joinable())
        {
            mediaServerThread.join();
            std::cout << "Server stopped" << '\n';
        }
    }
    return 0;
}