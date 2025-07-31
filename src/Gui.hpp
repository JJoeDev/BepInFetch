#pragma once

#include "ModManager.hpp"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

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

    std::vector<std::string> m_trackedMods{};

    bool m_showAbout{false};
    bool m_debugOverlay{false};
    bool m_showDemoWindow{false};
};
