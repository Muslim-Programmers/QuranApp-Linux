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
    src/interface.cpp
    src/audio_stream.cpp
    src/parser.cpp
    src/window.hpp
    src/interface.hpp
    src/audio_stream.hpp
    src/parser.hpp
    src/translations.hpp
)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(qapp ${SRCS})

target_link_libraries(qapp Qt5::Widgets mpg123 ao curl)
