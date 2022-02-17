cmake_minimum_required(VERSION 3.20.0)
project(DiscordFontChanger VERSION 1.0.0)

# General
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Support
if (NOT WIN32)
    message(FATAL_ERROR "Windows is currently the only supported platform.")
endif()

# Executable
add_executable(DiscordFontChanger "src/main.cpp")