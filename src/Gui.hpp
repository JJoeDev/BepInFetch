#pragma once

#include "ModManager.hpp"

#include <GLFW/glfw3.h>
#include <future>
#include <string>

class Gui {
public:
    explicit Gui(GLFWwindow* window, const char* glslVersion, ModManager* manager);
    ~Gui();

    void Render();

private:
    GLFWwindow* m_window{nullptr}; // Cannot be made const
    ModManager* m_modManager{nullptr};

    int m_rootWinFlags{};

    std::string m_enteredUrl{};

    std::unordered_map<int, modData> m_retrievedData{};
    std::unordered_map<int, std::future<modData>> m_futures{};
    std::future<void> m_downloadFuture{};
    bool m_retrivingData{false};

    // File Menu Items
    bool m_showAbout{false};
    bool m_debugOverlay{false};
    bool m_showDemoWindow{false};

    // Help Menu Items
    bool m_showHelp{false};
};
