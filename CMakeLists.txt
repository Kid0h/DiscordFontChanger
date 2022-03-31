cmake_minimum_required(VERSION 3.20.0)
project(DiscordFontChanger VERSION 1.0.0)

# General
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# WIN32 Runtime
if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Linux no-pie
if (NOT WIN32 AND (UNIX AND NOT APPLE))
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
endif()

# Support
if (NOT WIN32 AND NOT (UNIX AND NOT APPLE))
    message(FATAL_ERROR "Your platform is currently not supported.")
endif()

# Executable
add_executable(DiscordFontChanger "src/main.cpp")
