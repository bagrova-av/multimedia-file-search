#ifndef MEDIA_SERVER_H
#define MEDIA_SERVER_H

#include <string>
#include <mutex>
#include <httplib.h>

class MediaServer
{
public:
    MediaServer(const std::string& host, int port);
    
    void updateData(std::string newJson);
    void run();
    void stop();

private:
    httplib::Server server;
    std::string currentJson;
    std::mutex dataMutex;
};

#endif