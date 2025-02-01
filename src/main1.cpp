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

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "3D Model Viewer", monitor, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    InitImGui(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, mode->width, mode->height);

    Shader shader("D:/3DEngine/shaders/vertex.glsl", "D:/3DEngine/shaders/fragment.glsl");
    Model model("D:/3DEngine/assests/sofa.glb");

    if (model.isEmpty()) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        RenderImGui();

        // ImGui window setup
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(
            static_cast<float>(ImGui::GetIO().DisplaySize.x) * 0.7f,
            static_cast<float>(ImGui::GetIO().DisplaySize.y)
        ));
        ImGui::Begin("Render Window", nullptr, 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoTitleBar
        );
        
        ImVec2 renderSize = ImGui::GetContentRegionAvail();
        
        // Control panel
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
            ImGuiWindowFlags_NoResize
        );

        // ... GUI controls ...

        ImGui::End();

        // Viewport handling
        glViewport(0, 0, 
            static_cast<GLsizei>(renderSize.x), 
            static_cast<GLsizei>(renderSize.y));
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

        shader.use();
        camera.updateCameraVectors();

        // Projection matrix
        glm::mat4 projection;
        const float aspect = static_cast<float>(renderSize.x) / static_cast<float>(renderSize.y);
        
        if(usePerspective) {
            projection = glm::perspective(
                glm::radians(camera.fov), 
                aspect, 
                0.1f, 
                1000.0f
            );
        } else {
            projection = glm::ortho(
                -orthoSize * aspect,
                orthoSize * aspect,
                -orthoSize,
                orthoSize,
                0.1f,
                1000.0f
            );
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

        // Draw model
        model.draw(shader);

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ShutdownImGui();
    glfwTerminate();
    return 0;
}
