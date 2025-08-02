#include "Gui.hpp"
#include "ModManager.hpp"
#include "Theme.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_stdlib.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <future>
#include <string>

Gui::Gui(GLFWwindow* window, const char* glslVersion, ModManager* manager) : m_window(window), m_modManager(manager) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupImGuiStyle();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    m_rootWinFlags  = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
    m_rootWinFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

    m_modManager->SetOnDataRetrieving([this]() {
        std::cout << "OnDataRetrieving!\n";
        m_retrivingData = true;
    });

    m_modManager->SetOnDataRetrieved([this]() {
        std::cout << "OnDataRetrieved!\n";
        m_retrivingData = false;
    });
}

Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::Render() {
    if(m_downloadFuture.valid() && m_downloadFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        m_downloadFuture.get();
    }

    for(auto it = m_futures.begin(); it != m_futures.end(); ) {
        auto& future = it->second;

        if(future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            m_retrievedData[it->first] = future.get();
            it = m_futures.erase(it);
        }
        else {
            ++it;
        }
    }

    ImGui::SetNextWindowPos({0, 0}); 
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("root", nullptr, m_rootWinFlags);

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Quit", "Ctrl+Q")) { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }
            ImGui::Separator();
#ifndef NDEBUG
            ImGui::MenuItem("Toggle Demo Window", nullptr, &m_showDemoWindow);
#endif            
            ImGui::MenuItem("About", nullptr, &m_showAbout);
            ImGui::MenuItem("Debug Overlay", nullptr, &m_debugOverlay);

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("How to use", nullptr, &m_showHelp);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    float buttonWidth = ImGui::CalcTextSize("Track").x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float inputWidth = ImGui::GetContentRegionAvail().x - buttonWidth - spacing;

    ImGui::SetNextItemWidth(inputWidth);
    bool isEnter = ImGui::InputTextWithHint("##UrlTrackInput", "Enter GitHub URL to track", &m_enteredUrl, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if(ImGui::Button("Track") || isEnter) {
        if(!m_enteredUrl.empty()) {
            m_modManager->m_trackedMods.push_back(m_enteredUrl);
            m_enteredUrl = "";
        }
    }

    static int selected = 0;
    { // Left side
        ImGui::BeginGroup();
        ImGui::BeginChild("Mod List", {350, -ImGui::GetFrameHeightWithSpacing()}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

        if(m_modManager->m_trackedMods.size() > 0) {
            for(size_t i = 0; i < m_modManager->m_trackedMods.size(); ++i) {
                if(ImGui::Selectable(m_modManager->m_trackedMods[i].c_str(), selected == static_cast<int>(i))) {
                    selected = static_cast<int>(i);
                }
            }
        }

        ImGui::EndChild();
        if(m_modManager->m_trackedMods.size() > 0) {
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.686274509804f, 0.188235294118f, 0.160784313725f, 1.0f}); // Flexoki red 600
            if(ImGui::Button("Remove Selected")) {
                auto& mods = m_modManager->m_trackedMods;
                mods.erase(std::find(mods.begin(), mods.end(), mods[static_cast<size_t>(selected)]));
            }
            ImGui::PopStyleColor();
        }

        ImGui::EndGroup();
    }
    ImGui::SameLine();
    { // Right side
        ImGui::BeginGroup();
        ImGui::BeginChild("Mod View", {0, -ImGui::GetFrameHeightWithSpacing()}, ImGuiChildFlags_Borders);

        if(m_modManager->m_trackedMods.size() > 0) {
            const auto& data = m_retrievedData[selected];
            int sel = selected;
            if(ImGui::Button("Retrieve release data")) {
                m_futures[sel] = std::async(std::launch::async, [this, sel]() {
                    return m_modManager->GetReleaseData(m_modManager->m_trackedMods[static_cast<size_t>(sel)]);
                });
            }

            ImGui::Text("Mod Author: %s", data.uploader.c_str());
            ImGui::Text("Mod Name: %s", data.modName.c_str());
            ImGui::Text("Download URL: %s", data.downloadUrl.c_str());

            const std::string btnStr{"Download " + data.modName};
            if(ImGui::Button(btnStr.c_str())) {
                m_downloadFuture = std::async(std::launch::async, &ModManager::Download, m_modManager, data.downloadUrl, fs::current_path() / data.modName);
            }
        }

        ImGui::EndChild();
        if(m_retrivingData) {
            ImGui::ProgressBar(-1.0f * static_cast<float>(ImGui::GetTime()), {-FLT_MIN, 0}, "Retriving Data");
        }
        ImGui::EndGroup();
    }

    if(m_showAbout) { // A modal popup cannot open in a menu item. This is a workaround
        m_showAbout = false;
        ImGui::OpenPopup("About");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
    if(ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Name: BepInFetch");
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

    if(m_debugOverlay) {
        ImGui::SetNextWindowBgAlpha(0.55f);

        ImGui::Begin("DebugOverlay", &m_debugOverlay, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::Text("DEBUG OVERLAY");
        ImGui::Separator();

        ImGui::Text("FPS: %3.2f", ImGui::GetIO().Framerate);

        ImGui::End();
    }

#ifndef NDEBUG
    if(m_showDemoWindow) ImGui::ShowDemoWindow(&m_showDemoWindow);
#endif

    ImGui::End();
}
