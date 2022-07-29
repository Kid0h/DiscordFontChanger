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

# Fix Ubuntu no-pie
if (NOT WIN32 AND (UNIX AND NOT APPLE))
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
endif()

# Sources
file(GLOB SOURCES
    "src/*.cpp"
    "src/*.c"

    "vendor/glad/src/glad.c"

    "vendor/imgui/*.cpp"

    "vendor/base64/base64.c"
)

# Executable
if(WIN32)
    add_executable(DiscordFontChanger WIN32 "src/main.cpp")
else()
    add_executable(DiscordFontChanger "src/main.cpp")
endif()
target_sources(DiscordFontChanger PRIVATE ${SOURCES})

# Support
if (NOT WIN32 AND NOT (UNIX AND NOT APPLE))
    message(FATAL_ERROR "Your platform is currently not supported.")
endif()

# glad
set(GLAD_INCLUDE_DIR "vendor/glad/include")

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
add_subdirectory("vendor/glfw")

# imgui
set(IMGUI_INCLUDE_DIR "vendor/imgui")

# Includes
target_include_directories(DiscordFontChanger PRIVATE ${GLAD_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR})

# Links
target_link_libraries(DiscordFontChanger glfw)