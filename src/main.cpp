#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <filesystem>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#include "font_module.h"

#define DEBUG_LOG(x) if (debug) { x; std::flush(std::cout); }

void usage();
bool find_discord_core(std::filesystem::path& discord_path);
const char* type_type_to_name(const char* client);

int main(int argc, char* argv[]) {
    bool debug = false;
    bool reset = false;
    std::string client_name;
    const char* client = nullptr;
    const char* font = nullptr;

    // Arguments
    if (argc < 2) { usage(); return -1; }
    for (int i = 1; i < argc; ++i) {
        // Client
        if (strcmp(argv[i], "--client") == 0 || strcmp(argv[i], "-c") == 0) {
            if ((i == (argc - 1)) || (i == (argc - 2))) { usage(); return -1; }
            client = argv[++i];
        }
        // Reset
        else if (strcmp(argv[i], "--reset") == 0 || strcmp(argv[i], "-r") == 0) {
            reset = true;
        }
        // Debug
        else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            if (i == argc) { usage(); return -1; }
            debug = true;
        }
        // Help / Usage
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) { usage(); return 0; }
        else if (i == (argc - 1)) { font = argv[i]; }
        else { usage(); return -1; }
    }

    /* Get neccesarry paths */
    bool discord_found = false;
    bool module_found = false;
    std::error_code ec;
    std::filesystem::path module_base, module_path, index_path;
    #ifdef WIN32
    module_base = std::filesystem::temp_directory_path(ec).parent_path().parent_path();
    #elif __linux__
    struct passwd *pw = getpwuid(getuid());
    module_base = std::string(pw->pw_dir) + "/.config";
    #endif

    // Discord client
    client_name = type_type_to_name(client);
    if (client_name.length() == 0) {
        std::cout << "ERROR: Invalid client type.\n";
        usage();
        return -1;
    }
    DEBUG_LOG(std::cout << "Searching for " << client_name << "..");
    for (uint16_t i = 0; i < 3; ++i) {
        std::string name;
        std::filesystem::path base = module_base;
        bool force = client;

        if ((force && (strcmp(client, "stable") == 0)) || (!force && i == 0)) { // Discord
            if (!force) { name = type_type_to_name("stable"); }
            #ifdef WIN32
            base /= "Discord";
            #elif __linux__
            base /= "discord";
            #endif
        } else if ((force && (strcmp(client, "canary") == 0)) || (!force && i == 1)) { // Discord Canary
            if (!force) { name = type_type_to_name("canary"); }
            #ifdef WIN32
            base /= "DiscordCanary";
            #elif __linux__
            base /= "discordcanary";
            #endif
        } else if ((force && (strcmp(client, "ptb") == 0)) || (!force && i == 2)) { // Discord PTB
            if (!force) { name = type_type_to_name("ptb"); }
            #ifdef WIN32
            base /= "DiscordPTB";
            #elif __linux__
            base /= "discordptb";
            #endif
        }

        if (std::filesystem::exists(base) && find_discord_core(base)) {
            discord_found = true;
            module_base = base;
            module_base /= "modules";
            if (!force) { client_name = name; }
            break;
        } else if (force) { // Force specific client
            break;
        }
    }
    if (discord_found) {
        DEBUG_LOG(std::cout << " found " << client_name << "!" << std::endl);
        // Module directory
    } else {
        DEBUG_LOG(std::cout << " not found, is " << client_name << " installed?" << std::endl) else { std::cout << "Could not find " << client_name << ", is it installed?" << std::endl; }
        return -1;
    }

    // Module path
    DEBUG_LOG(std::cout << "Searching target module path..");
    for (auto& dir : std::filesystem::directory_iterator(module_base, ec)) {
        if (dir.is_directory() && dir.path().filename().wstring().rfind(L"discord_voice", 0) == 0) {
            module_found = true;
            #ifdef WIN32
            module_base = dir.path(); module_base /= "discord_voice"; module_base /= "node_modules"; module_base /= "signal-exit";
            #elif __linux__
            module_base /= "discord_voice"; module_base /= "node_modules"; module_base /= "signal-exit";
            #endif
            break;
        }
    }
    if (module_found) {
        DEBUG_LOG(std::cout << " found!" << std::endl);
    } else {
        DEBUG_LOG(std::cout << " not found, is " << client_name << " installed?" << std::endl) else { std::cout << "Could not find " << client_name << "'s module path" << std::endl; }
        return -1;
    }

    index_path = module_base; index_path /= "index.js"; // Index path
    module_path = module_base; module_path /= FONT_MODULE_FILENAME; // Module path

    // Sanity check
    if (!std::filesystem::exists(index_path, ec)) {
        std::cout << "Could not find " << client_name << "'s module entry path, is " << client_name << " installed?" << std::endl;
        return -1;
    }

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
    DEBUG_LOG(std::cout << "Searching " << client_name << " for injected font module..");
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
    std::cout << "Restart " << client_name << " to apply changes!" << std::endl;

    return 0;
}

void usage() {
    std::cout << "Usage: DiscordFontChanger.exe"
    << " [options] <font>" << "\n"
    << "  -c, --client      Target a specific Discord client (stable, canary, ptb)\n"
    << "  -r, --reset       Reset Discord's font to the default\n"
    << "  -d, --debug       Enable debug messages\n"
    << "  -h, --help        Disaply this help message"
    << std::endl;
}

bool find_discord_core(std::filesystem::path& discord_path) {
    std::error_code ec;
    bool found = false;

    for (auto& dir : std::filesystem::directory_iterator(discord_path, ec)) {
        #ifdef WIN32
        if (dir.is_directory() && dir.path().filename().wstring().rfind(L"app-1", 0) == 0) {
        #elif __linux__
        if (dir.is_directory() && dir.path().filename().wstring().rfind(L"0.", 0) == 0) {
        #endif
            found = true;
            discord_path = dir.path();
            break;
        }
    }

    return found;
}

const char* type_type_to_name(const char* client) {
    if (!client || strcmp(client, "stable") == 0) {
        return "Discord";
    }
    else if (strcmp(client, "canary") == 0) {
        return "Discord Canary";
    }
    else if (strcmp(client, "ptb") == 0) {
        return "Discord PTB";
    }

    return "";
}