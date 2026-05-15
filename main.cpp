#include <iostream>

#include "./MediaScanner/MediaScanner.h"

int main()
{
    MediaScanner mediaScanner("/home/user");
    ScanResult result = mediaScanner.scan();
    for (const auto item : result)
    {
        std::cout << item.first << '\n';
        for (const auto file : item.second)
        {
            std::cout << file << '\n';
        }

        
    }
}