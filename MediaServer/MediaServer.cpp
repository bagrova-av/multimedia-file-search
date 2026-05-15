#include "MediaServer.h"

#include <iostream>

MediaServer::MediaServer(const std::string& host, int port)
{
    server.Get("/media_files", [this](const httplib::Request& request, httplib::Response& response) 
    {
        std::lock_guard<std::mutex> lock(dataMutex);
        
        response.set_content(currentJson, "application/json");
        std::cout << "HTTP: Sent response to " << request.remote_addr << '\n';
    });
}

void MediaServer::updateData(std::string newJson)
{
    std::lock_guard<std::mutex> lock(dataMutex);
    currentJson = std::move(newJson);
}

void MediaServer::run()
{
    std::cout << "Server is starting on localhost:1234" << '\n';
    if (!server.listen("0.0.0.0", 1234))
    {
        std::cerr << "Error: could not start HTTP server" << '\n';
    }
}

void MediaServer::stop()
{
    server.stop();
}