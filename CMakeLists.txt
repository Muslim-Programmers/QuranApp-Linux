cmake_minimum_required(VERSION 3.9)
project(Quran-app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SRCS
    src/main.cpp
    src/window.cpp
    src/curl_parser.cpp
    src/window.hpp
    src/curl_parser.hpp
    src/db_reader.cpp 
    src/db_reader.hpp
)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_executable(qapp ${SRCS})

target_link_libraries(qapp Qt5::Widgets Qt5::Multimedia curl sqlite3)
