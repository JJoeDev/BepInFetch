#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Gui {
public:
    explicit Gui(GLFWwindow* window, const char* glslVersion);
    ~Gui();

    void Render();

private:
    GLFWwindow* m_window{nullptr}; // Cannot be made const

    int m_rootWinFlags{};

    std::string m_enteredUrl{};

    bool m_showAbout{false};
    bool m_showDemoWindow{false};
};
