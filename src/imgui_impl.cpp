// Dear ImGui integration implementation for OpenGL3/GLFW
// Handles GUI system initialization, rendering, and shutdown
#include "imgui_impl.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// Initialize ImGui context and link with GLFW/OpenGL
// window: GLFW window handle to attach to

void InitImGui(GLFWwindow* window) {
    // Safety check for version mismatches between compiled code and linked library
    IMGUI_CHECKVERSION();
     // Create main ImGui context (stores all GUI state)
    ImGui::CreateContext();
    // Get reference to IO subsystem for configuration
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);

    // Initialize Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
// Prepare ImGui for new frame rendering
// Call before any ImGui::Begin/End blocks
void RenderImGui() {
    // Start building new UI frame in this order:
    // 1. Renderer backend (OpenGL3)
    // 2. Platform backend (GLFW)
    // 3. ImGui itself
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
// Cleanup ImGui resources in reverse initialization order
void ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
