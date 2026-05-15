cmake_minimum_required(VERSION 3.14)
project(MediaScanner)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_LIST
    main.cpp
    MediaScanner/MediaScanner.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE
    MediaScanner
)