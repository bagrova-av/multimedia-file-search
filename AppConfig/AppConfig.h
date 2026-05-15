#pragma once
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>

struct AppConfig
{
    std::string targetPath;
    int intervalSeconds = 10;
    std::string configFilePath;
};

#endif

