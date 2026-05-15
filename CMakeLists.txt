cmake_minimum_required(VERSION 3.14)
project(MediaScanner)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

set(SOURCE_LIST
    main.cpp
    MediaScanner/MediaScanner.cpp
)
add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE
    MediaScanner
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)