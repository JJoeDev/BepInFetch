#pragma once

#include "Gui.hpp"
#include "ModManager.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <string>

struct AppSpecs {
    int width{800};
    int height{600};
    std::string title{};
};

class Application {
public:
    explicit Application(const AppSpecs& specs);
    ~Application();

    void Run();

private:
    GLFWwindow* m_window{nullptr};

    std::unique_ptr<Gui> m_gui{nullptr};
    ModManager m_modManager{};

    const char* m_GLSL_VERSION{"#version 440"};
};
