#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "model.h"
#include "shader.h"
#include "imgui_impl.h"
#include "camera.h"
#include "input.h"
#include "globals.h"
// Near and far planes for projection matrix
float nearPlane = 0.1f; // Minimum visible distance
float farPlane = 1000.0f;// Maximum visible distance
// Initial light position in world space
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
    // Initialize GLFW windowing system
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
// Configure GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA anti-aliasing
// Create fullscreen window
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "3D Model Viewer", monitor, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
 // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
// Initialize ImGui with custom style
    InitImGui(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, mode->width, mode->height);

    Shader shader("D:/3DEngine/shaders/vertex.glsl", "D:/3DEngine/shaders/fragment.glsl");
    Model model("D:/3DEngine/assests/colgate.glb");

    if (model.isEmpty()) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        // Start the Dear ImGui frame
        RenderImGui();

        // 1. Control Panel Window (30% width)
        ImGui::SetNextWindowPos(ImVec2(
            static_cast<float>(ImGui::GetIO().DisplaySize.x) * 0.7f, 
            0.0f
        ));
        ImGui::SetNextWindowSize(ImVec2(
            static_cast<float>(ImGui::GetIO().DisplaySize.x) * 0.3f,
            static_cast<float>(ImGui::GetIO().DisplaySize.y)
        ));
        ImGui::Begin("Control Panel", nullptr, 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse
        );

        // Control Panel Content
        ImGui::Checkbox("Use Perspective", &usePerspective);

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader("Projection Settings")) {
            if(usePerspective) {
                ImGui::SliderFloat("FOV", &camera.fov, 1.0f, 120.0f);
            } else {
                ImGui::SliderFloat("Ortho Size", &orthoSize, 1.0f, 100.0f);
            }
            ImGui::SliderFloat("Near Plane", &nearPlane, 0.1f, 10.0f);
            ImGui::SliderFloat("Far Plane", &farPlane, 10.0f, 1000.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader("Camera Settings")) {
            ImGui::SliderFloat("Position X", &camera.cameraPos.x, -10.0f, 10.0f);
            ImGui::SliderFloat("Position Y", &camera.cameraPos.y, -10.0f, 10.0f);
            ImGui::SliderFloat("Position Z", &camera.cameraPos.z, -10.0f, 10.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader("Model Transforms")) {
            ImGui::SliderFloat3("Position", &modelPosition.x, -5.0f, 5.0f);
            ImGui::SliderFloat3("Rotation", &modelRotation.x, -180.0f, 180.0f);
            ImGui::SliderFloat("Scale", &modelScale, 0.1f, 5.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader("Lighting")) {
            ImGui::SliderFloat3("Light Position", &lightPos.x, -10.0f, 10.0f);
        }

        ImGui::Checkbox("Wireframe Mode", &wireframeMode);
        ImGui::End();
        

        // 2. Render View Window (70% width)
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(
            static_cast<float>(ImGui::GetIO().DisplaySize.x) * 0.7f,
            static_cast<float>(ImGui::GetIO().DisplaySize.y)
        ));
        ImGui::Begin("Render View", nullptr, 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground
        );

        ImVec2 renderSize = ImGui::GetContentRegionAvail();
        glViewport(0, 0, 
            static_cast<GLsizei>(renderSize.x), 
            static_cast<GLsizei>(renderSize.y));
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

        // 3D Rendering
        shader.use();
        camera.updateCameraVectors();

        // Projection matrix
        glm::mat4 projection;
        const float aspect = static_cast<float>(renderSize.x) / static_cast<float>(renderSize.y);
        
        if(usePerspective) {
            projection = glm::perspective(glm::radians(camera.fov), aspect, nearPlane, farPlane);
        } else {
            projection = glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, nearPlane, farPlane);
        }

        // View matrix
        glm::mat4 view = camera.getViewMatrix();
        
        // Model matrix
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, modelPosition);
        model_matrix = glm::rotate(model_matrix, glm::radians(modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::scale(model_matrix, glm::vec3(modelScale));

        // Set shader uniforms
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model_matrix);
        shader.setVec3("viewPos", camera.cameraPos);
        shader.setVec3("lightPos", lightPos);

        // Draw model
        model.draw(shader);

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ShutdownImGui();
    glfwTerminate();
    return 0;
}
