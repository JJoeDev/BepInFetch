#include "Gui.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

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
            ImGui::Separator();
#ifndef NDEBUG
            if(ImGui::MenuItem("Toggle Demo Window")) { m_showDemoWindow = !m_showDemoWindow; }
#endif            
            if(ImGui::MenuItem("About")) { m_showAbout = true; }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if(m_showAbout) { // A modal popup cannot open in a menu item. This is a workaround
        m_showAbout = false;
        ImGui::OpenPopup("About");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
    if(ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Name: BepFetch");
        ImGui::Text("Version: 0.0.1");
        ImGui::Text("Copyright 2025 JJoeDev");
        ImGui::Separator();
        ImGui::Text("BepFetch was created by JJoeDev at github.com/jjoedev");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextWrapped("This application is a Mod Manager meant for mods using BepInEx. and so on...");

        float btnWidth = ImGui::GetContentRegionAvail().x;
        if(ImGui::Button("Close", {btnWidth, 0})) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

#ifndef NDEBUG
    if(m_showDemoWindow) ImGui::ShowDemoWindow(&m_showDemoWindow);
#endif

    ImGui::End();
}
