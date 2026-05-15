#include "utils.h"

#include <fstream>
#include <iostream>

#include <algorithm>

std::string toLowerCase(const std::string& str)
{
    std::string lowerStr = str;
    std::transform(str.cbegin(), str.cend(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void saveJsonToFile(const std::string& path, const std::string& content)
{
    std::ofstream outFile(path);
    if (outFile.is_open())
    {
        outFile << content;
        outFile.close();
        std::cout << "JSON saved to: " << path << '\n';
    }
    else
    {
        std::cerr << "Error: could not write to file " << path << '\n';
    }
}