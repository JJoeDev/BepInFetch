#pragma once

#include "ConfigManager.hpp"
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

    [[nodiscard]]
    const inline fs::path& GetDownloadPath() const { return m_downloadDest; }
    inline void SetDownloadPath(const fs::path& path) { m_downloadDest = path; }

    [[nodiscard]]
    const inline ConfigManager& GetCfgManager();

private:
    GLFWwindow* m_window{nullptr};

    std::unique_ptr<Gui> m_gui{nullptr};
    ModManager m_modManager{};
    ConfigManager m_configManager{};

    fs::path m_downloadDest{};

    const char* m_GLSL_VERSION{"#version 440"};
};
