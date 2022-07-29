#include <iostream>
#include <string>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"

#include "api.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// GLFW error callback
static void glfw_error_callback(int error, const char *description) {
    return;
}
static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main(int argc, char* argv[]) {
#endif
    bool debug      = false;
    bool reset      = false;
    char client[7]  = { 0 };
    std::string font_name; font_name.resize(255);

    // GLFW Setup
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    // Window settings
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    /* OpenGL + GLSL versions */
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create & use window
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Discord Font Changer", NULL, NULL);
    if (!window) {
        return -1;
    } glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // VSync
    glfwSwapInterval(1);

    // Load OpenGL
    gladLoadGL();

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr; io.LogFilename = nullptr;

    // ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main window size
    bool main_window_size_good = false;
    ImVec2 main_window_size = ImVec2(0.0f, 0.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Poll for event
        glfwPollEvents();

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window
        {
            // ImGui::SetNextWindowPos(ImVec2(0.0f, menu_bar_size.y));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {380.0f,69.0f});
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowBgAlpha(0.85f);
            if (main_window_size_good) { ImGui::SetNextWindowSize(main_window_size); }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            ImGui::Begin("main", nullptr,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | 
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse
            );

            // Status
            static std::string status_text = "Unchaged.";
            enum StatusColor {
                normal,
                gray,
                green,
                red
            } static status_color = StatusColor::gray;

            {
                ImGui::Text("Status:"); ImGui::SameLine();
                static ImVec4 text_color = ImVec4();
                switch (status_color)
                {
                case StatusColor::normal:
                    text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                
                case StatusColor::gray:
                    text_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                    break;
                    
                case StatusColor::green:
                    text_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    break;

                case StatusColor::red:
                    text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    break;

                default:
                    text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                }

                ImGui::TextColored(text_color, "%s", status_text.c_str());
            }

            // Client type
            static bool auto_selected           = true;
            {
                static bool stable_selected         = false;
                static bool canary_selected         = false;
                static bool ptb_selected            = false;
                static bool client_type_menu_open   = false;
                static bool want_open               = false;
                static std::string client_type_name = "Automatic";
                
                ImGui::Text("Client:");
                ImGui::SameLine();
                if (ImGui::BeginMenu(client_type_name.c_str(), &client_type_menu_open)) {
                    if (ImGui::MenuItem("Automatic", nullptr, &auto_selected)) {
                        client_type_name = "Automatic";
                        stable_selected = false;
                        canary_selected = false;
                        ptb_selected    = false;
                        if (!auto_selected) { auto_selected = true; }
                        strcpy(client, "");
                    } if (ImGui::MenuItem("Stable", nullptr, &stable_selected)) {
                        client_type_name = "Stable";
                        canary_selected = false;
                        ptb_selected    = false;
                        auto_selected   = false;
                        if (!stable_selected) { stable_selected = true; }
                        strcpy(client, "stable");
                    } if (ImGui::MenuItem("Canary", nullptr, &canary_selected)) {
                        client_type_name = "Canary";
                        stable_selected = false;
                        ptb_selected    = false;
                        auto_selected   = false;
                        if (!canary_selected) { canary_selected = true; }
                        strcpy(client, "canary");
                    } if (ImGui::MenuItem("PTB", nullptr, &ptb_selected)) {
                        client_type_name = "PTB";
                        stable_selected = false;
                        canary_selected = false;
                        auto_selected   = false;
                        if (!ptb_selected) { ptb_selected = true; }
                        strcpy(client, "ptb");
                    }
                    ImGui::EndMenu();
                }
            }

            // Profile input
            bool triggered = false;
            ImGui::Text("%s", "Font:");
            ImGui::SameLine();
            bool text_triggered = ImGui::InputText("##font_text_input", font_name.data(), font_name.capacity(), ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();
            bool change_button_triggered = ImGui::Button("Change");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1.0f);
            bool reset_button_triggered = ImGui::Button("Reset");
            ImGui::PopItemWidth();

            static bool success = false;
            static std::string message;
            if ((triggered = reset_button_triggered || change_button_triggered || text_triggered)) {
                // Change font
                std::stringstream output;
                if (API::change(font_name.c_str(), (auto_selected ? nullptr : client), output, reset_button_triggered)) {
                    status_color = StatusColor::green;
                } else {
                    status_color = StatusColor::red;
                } status_text = output.str();
            }

            // Determine OS window size
            static size_t frame_num = 0;
            if (!main_window_size_good) {
                frame_num++;
            }
            if (!main_window_size_good && frame_num == 2) {
                main_window_size_good = true;
                main_window_size = ImGui::GetWindowSize();
                // main_window_size.y += menu_bar_size.y;

                glfwSetWindowSize(window, (int)main_window_size.x, (int)main_window_size.y); // Resize
                glfwShowWindow(window); // Show window
            }

            ImGui::End();
            ImGui::PopStyleVar(3);
        }

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        glfwSwapBuffers(window);
    }

    // Cleanup
    /* ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* GLFW */
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}