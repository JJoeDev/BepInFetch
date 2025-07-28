#include "Gui.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Gui::Gui(GLFWwindow* window, const char* glslVersion) : m_window(window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    m_rootWinFlags  = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
    m_rootWinFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
}

Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::Render() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("root", nullptr, m_rootWinFlags);

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Quit")) { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }
#ifndef NDEBUG
            if(ImGui::MenuItem("Toggle Demo Window")) { m_showDemoWindow = !m_showDemoWindow; }
#endif            

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

#ifndef NDEBUG
    if(m_showDemoWindow) ImGui::ShowDemoWindow(&m_showDemoWindow);
#endif

    ImGui::End();
}
