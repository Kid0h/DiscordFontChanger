
#include <iostream>
#include <fstream>
#include <filesystem>

#include "font_module.h"

#define DEBUG_LOG(x) if (debug) { x; }

void usage();

int main(int argc, char* argv[]) {
    bool debug = false;
    bool reset = false;
    const char* font = nullptr;

    // Arguments
    if (argc < 2) { usage(); return -1; }
    for (int i = 1; i < argc; ++i) {
        // Help
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) { usage(); return 0; }
        // Reset
        else if (strcmp(argv[i], "--reset") == 0 || strcmp(argv[i], "-r") == 0) {
            reset = true;
        }
        // Debug
        else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            if (&argv[i] == &argv[1]) { usage(); return -1; }
            debug = true;
        }
        // Font
        else if (i == 1) {
            font = argv[i];
        }
        // Usage
        else { usage(); return -1; }
    }
    // if (!reset) {
    //     font = argv[1];
    // }

    /* Get neccesarry paths */
    // Module base
    bool discord_found = false;
    std::error_code ec;
    std::filesystem::path executable_path, module_base, module_path, index_path;
    module_base = std::filesystem::temp_directory_path(ec).parent_path().parent_path() /= "Discord";

    DEBUG_LOG(std::cout << "Searching Discord's path..");
    if (!std::filesystem::exists(module_base, ec)) {  }
    for (auto& dir : std::filesystem::directory_iterator(module_base, ec)) {
        if (dir.is_directory() && dir.path().filename().wstring().rfind(L"app-1", 0) == 0) {
            discord_found = true;
            executable_path = module_base = dir.path();
            break;
        }
    }
    if (discord_found) {
        // Executable path
        executable_path /= "Discord.exe";

        // Module directory
        DEBUG_LOG(std::cout << " found!" << std::endl);
        module_base /= "modules"; module_base /= "discord_voice-5"; module_base /= "discord_voice"; module_base /= "node_modules"; module_base /= "signal-exit";
    } else {
        DEBUG_LOG(std::cout << " not found, is Discord installed?" << std::endl) else { std::cout << "Could not find Discord's path"; }
        return -1;
    }

    index_path = module_base; index_path /= "index.js"; // Index path
    module_path = module_base; module_path /= FONT_MODULE_FILENAME; // Module path

    /* Font Module */
    // Create/Delete Font module
    bool font_module_exists = std::filesystem::exists(module_path, ec);
    if (!reset) {
        DEBUG_LOG(std::cout << ( font_module_exists ? "Editing" : "Creating") << " custom font module.." << std::endl);
        std::fstream f(module_path, std::ios::out | std::ios::trunc);
        if (f.is_open()) {
            f << "var font = \""; f << font << "\";\n";
            f << font_module;
        } f.close();
    }
    else {
        if (font_module_exists) {
            std::filesystem::remove(module_path, ec);
        } DEBUG_LOG(std::cout << "Deleted font module" << std::endl);
    }

    /* Module injection/ejection */
    // Check if already injected
    DEBUG_LOG(std::cout << "Searching Discord for injected font module..");
    bool already_injected = false;
    size_t inject_pos_start, inject_pos_end; {
        inject_pos_start    = std::string::npos;
        inject_pos_end      = std::string::npos;
        std::ifstream f_i(index_path, std::ios::in);
        if (f_i.is_open()) {
            size_t inject_start_line_len = strlen(inject_code_start);
            size_t inject_end_line_len = strlen(inject_code_end);
            size_t max_line_len = std::max(inject_start_line_len, inject_end_line_len);

            size_t line_num = 0;
            char* line = (char*)calloc(max_line_len + 1, sizeof(char)); 
            while (true) {
                // Get line
                f_i.getline(line, max_line_len + 1);
                if (!f_i.fail()) { line_num++; }
                
                // Inject start sig
                if (strcmp(line, inject_code_start) == 0) {
                    inject_pos_start = line_num;
                }
                // Inject end sig
                else if (strcmp(line, inject_code_end) == 0) {
                    inject_pos_end = line_num;
                }

                // Stop reading if got to end
                if (f_i.eof()) {
                    break;
                } f_i.clear();
            }
            if (inject_pos_start != std::string::npos && inject_pos_end != std::string::npos) {
                DEBUG_LOG(std::cout << " found!";);
                already_injected = true;
            } else {
                DEBUG_LOG(std::cout << " not found");
            }
            free(line);
        } f_i.close();
    }

    // Eject module
    if (reset) {
        if (already_injected) {
            // Copy file but add only original lines (without font module)
            std::filesystem::path temp_index = index_path.wstring() + L".tmp";

            DEBUG_LOG(std::cout << " ejecting.." << std::endl);
            std::ifstream f_i(index_path);
            std::ofstream f_o(temp_index, std::ios::out);

            char c;
            size_t line_num = 1;
            while (f_i.get(c)) {
                if (c == '\n') {
                    line_num++;
                }

                if (line_num < inject_pos_start || line_num > inject_pos_end) {
                    f_o << c;
                }
            }

            f_o.close();
            f_i.close();
            
            std::filesystem::remove(index_path, ec);
            std::filesystem::rename(temp_index, index_path, ec);

        } else {
            DEBUG_LOG(std::cout << ", nothing to eject." << std::endl);
        }
        std::cout << "Font has been reset!" << std::endl;
    }
    // Inject module
    else {
        if (!already_injected) {
            DEBUG_LOG(std::cout << ", injecting.." << std::endl);
            std::fstream f(index_path, std::ios::app);
            if (f.is_open()) {
                f << "\n";
                f << inject_code_start << "\n";
                f << inject_code << "\n";
                f << inject_code_end;
            } f.close();
        } std::cout << "Font changed!" << std::endl;
    }

    /* Restart Discord */
    std::cout << "Restart Discord to apply changes!";

    return 0;
}

void usage() {
    std::cout << "Usage: DiscordFontChanger.exe"
    << " [FONT] [OPTION]" << "\n"
    << "  -r, --reset       Reset Discord's font to the default\n"
    << "  -d, --debug       Enable debug messages\n"
    << "  -h, --help        Disaply this help message"
    << std::endl;
}