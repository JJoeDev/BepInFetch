#pragma once

#include <GLFW/glfw3.h>

class Gui {
public:
    explicit Gui(GLFWwindow* window, const char* glslVersion);
    ~Gui();

    void Render();

private:
    GLFWwindow* m_window{nullptr}; // Cannot be made const

    int m_rootWinFlags{};

    bool m_showDemoWindow{false};
};
