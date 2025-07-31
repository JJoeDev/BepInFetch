#include "Application.hpp"

#include <GLFW/glfw3.h>
#include <gl/gl.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application::Application(const AppSpecs& specs) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(specs.width, specs.height, specs.title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    m_gui = std::make_unique<Gui>(m_window, m_GLSL_VERSION, &m_modManager);
}

Application::~Application() {
    m_gui.reset();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::Run() {
    while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        if(glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0 || glfwGetWindowAttrib(m_window, GLFW_FOCUSED) == 0) {
            ImGui_ImplGlfw_Sleep(30);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_gui->Render();

        ImGui::Render();

        int dpyWidth, dpyHeight;
        glfwGetFramebufferSize(m_window, &dpyWidth, &dpyHeight);
        glViewport(0, 0, dpyWidth, dpyHeight);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}
