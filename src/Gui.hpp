#pragma once

#include "ModManager.hpp"

#include <GLFW/glfw3.h>
#include <future>
#include <string>

#include "imgui.h"
#include "imfilebrowser.h"

class Gui {
public:
    explicit Gui(GLFWwindow* window, const char* glslVersion, ModManager* manager);
    ~Gui();

    /// For stuff that can't be initialized in the constructor
    void PostInit();

    void Render();

    [[nodiscard]]
    const inline std::unordered_map<int, ModData> GetModData() { return m_retrievedData; }

private:
    GLFWwindow* m_window{nullptr}; // Cannot be made const
    ModManager* m_modManager{nullptr};

    int m_rootWinFlags{};

    ImGui::FileBrowser m_fileBrowser{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_HideRegularFiles};

    std::string m_enteredUrl{};

    std::vector<std::string> m_trackedMods{};
    std::unordered_map<int, ModData> m_retrievedData{};
    std::unordered_map<int, std::future<ModData>> m_futures{};
    std::future<void> m_downloadFuture{};
    bool m_retrivingData{false};

    // File Menu Items
    bool m_showAbout{false};
    bool m_debugOverlay{false};
    bool m_showDemoWindow{false};

    // Help Menu Items
    bool m_showHelp{false};

    void SaveData();
    void ReadData();
};
