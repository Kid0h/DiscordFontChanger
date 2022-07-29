#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <string.h>
#include <fstream>
#include <filesystem>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#include "font_module.h"

class API {
public:
    API();
    ~API();

    static bool find_discord_core(std::filesystem::path& discord_path);
    static const char* type_type_to_name(const char* client);
    static bool change(const char* font, const char* client, std::stringstream& output_buffer, bool reset = false, std::stringstream* debug_output_buffer = nullptr);
};