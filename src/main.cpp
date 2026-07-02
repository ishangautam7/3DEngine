#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "model.h"
#include "shader.h"
#include "imgui_impl.h"
#include "camera.h"
#include "input.h"
#include "globals.h"

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#endif

// Projection parameters and light position
float nearPlane = 0.1f;
float farPlane = 1000.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Open a native file picker and return the chosen path (empty if cancelled)
static std::string openModelDialog() {
#ifdef _WIN32
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter =
        "3D Models\0*.glb;*.gltf;*.obj;*.fbx;*.dae;*.stl;*.ply;*.3ds\0"
        "All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn))
        return std::string(filename);
#endif
    return std::string();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Resizable windowed app (not fullscreen)
    GLFWwindow* window = glfwCreateWindow(1280, 720, "3D Model Viewer", nullptr, nullptr);
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

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader lineShader("shaders/line_vertex.glsl", "shaders/line_fragment.glsl");

    // Coordinate axis lines (X, Y, Z from the origin)
    float axisVertices[] = {
        0.0f, 0.0f, 0.0f, 10.0f,  0.0f,  0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 10.0f,  0.0f,
        0.0f, 0.0f, 0.0f,  0.0f,  0.0f, 10.0f
    };
    unsigned int axisVAO, axisVBO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Ground plane grid on the XZ plane
    std::vector<float> gridVertices;
    const int gridHalf = 10;
    for (int i = -gridHalf; i <= gridHalf; ++i) {
        gridVertices.insert(gridVertices.end(), {
            (float)i, 0.0f, (float)-gridHalf,  (float)i, 0.0f, (float)gridHalf,
            (float)-gridHalf, 0.0f, (float)i,   (float)gridHalf, 0.0f, (float)i
        });
    }
    int gridVertexCount = static_cast<int>(gridVertices.size() / 3);
    unsigned int gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    bool showGrid = true;
    bool showAxis = true;

    std::string modelPath = "assests/infernus.glb";
    std::unique_ptr<Model> model = std::make_unique<Model>(modelPath);
    if (model->isEmpty())
        std::cerr << "Failed to load default model: " << modelPath << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);
        RenderImGui();

        // Control Panel on the left (30% width)
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.3f, ImGui::GetIO().DisplaySize.y));
        ImGui::Begin("Control Panel", nullptr,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // Load a model from disk at runtime
        if (ImGui::Button("Open Model...")) {
            std::string file = openModelDialog();
            if (!file.empty()) {
                auto loaded = std::make_unique<Model>(file);
                if (!loaded->isEmpty()) {
                    model = std::move(loaded);
                    modelPath = file;
                } else {
                    std::cerr << "Failed to load model: " << file << std::endl;
                }
            }
        }
        ImGui::TextWrapped("Current: %s", modelPath.c_str());
        ImGui::Separator();

        ImGui::Checkbox("Use Perspective", &usePerspective);
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Projection Settings")) {
            if (usePerspective)
                ImGui::SliderFloat("FOV", &camera.fov, 1.0f, 120.0f);
            else
                ImGui::SliderFloat("Ortho Size", &orthoSize, 1.0f, 100.0f);
            ImGui::SliderFloat("Near Plane", &nearPlane, 0.1f, 10.0f);
            ImGui::SliderFloat("Far Plane", &farPlane, 10.0f, 1000.0f);
        }

        if (ImGui::CollapsingHeader("Appearance")) {
            ImGui::ColorEdit3("Background Color", &backgroundColor.r);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Camera Settings")) {
            ImGui::SliderFloat("Yaw", &camera.yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("Pitch", &camera.pitch, -89.0f, 89.0f);
            ImGui::SliderFloat("Distance", &camera.cameraDistance, 1.0f, 100.0f);
        }

        if (ImGui::CollapsingHeader("Model Transforms")) {
            ImGui::SliderFloat3("Position##Model", &modelPosition.x, -5.0f, 5.0f);
            ImGui::SliderFloat3("Rotation", &modelRotation.x, -180.0f, 180.0f);
            ImGui::SliderFloat("Scale", &modelScale, 0.1f, 5.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Lighting")) {
            ImGui::SliderFloat3("Light Position", &lightPos.x, -10.0f, 10.0f);
        }

        if (ImGui::CollapsingHeader("Scene")) {
            ImGui::Checkbox("Show Ground Plane", &showGrid);
            ImGui::Checkbox("Show Axis", &showAxis);
        }

        ImGui::Checkbox("Wireframe Mode", &wireframeMode);
        ImGui::End();

        // Render View on the right (70% width)
        float panelWidth = ImGui::GetIO().DisplaySize.x * 0.3f;
        ImGui::SetNextWindowPos(ImVec2(panelWidth, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.7f, ImGui::GetIO().DisplaySize.y));
        ImGui::Begin("Render View", nullptr,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

        // Map the render region to a GL viewport (handles window resize and DPI)
        ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 renderSize = ImGui::GetContentRegionAvail();

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        float scaleX = fbWidth / ImGui::GetIO().DisplaySize.x;
        float scaleY = fbHeight / ImGui::GetIO().DisplaySize.y;
        float originX = (winPos.x + regionMin.x) * scaleX;
        float originY = (winPos.y + regionMin.y) * scaleY;

        glViewport(static_cast<GLint>(originX),
                   static_cast<GLint>(fbHeight - originY - renderSize.y * scaleY),
                   static_cast<GLsizei>(renderSize.x * scaleX),
                   static_cast<GLsizei>(renderSize.y * scaleY));

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

        shader.use();
        camera.updateCameraVectors();

        float aspect = renderSize.y > 0.0f ? renderSize.x / renderSize.y : 1.0f;
        glm::mat4 projection = usePerspective ?
            glm::perspective(glm::radians(camera.fov), aspect, nearPlane, farPlane) :
            glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, nearPlane, farPlane);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, modelPosition);
        model_matrix = glm::rotate(model_matrix, glm::radians(modelRotation.x), glm::vec3(1, 0, 0));
        model_matrix = glm::rotate(model_matrix, glm::radians(modelRotation.y), glm::vec3(0, 1, 0));
        model_matrix = glm::scale(model_matrix, glm::vec3(modelScale));

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model_matrix);
        shader.setVec3("viewPos", camera.cameraPos);
        shader.setVec3("lightPos", lightPos);

        if (model)
            model->draw(shader);

        // Draw the ground plane and axis overlays in world space
        if (showGrid || showAxis) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            lineShader.use();
            lineShader.setMat4("projection", projection);
            lineShader.setMat4("view", view);

            if (showGrid) {
                lineShader.setVec3("lineColor", glm::vec3(0.4f, 0.4f, 0.4f));
                glBindVertexArray(gridVAO);
                glDrawArrays(GL_LINES, 0, gridVertexCount);
            }
            if (showAxis) {
                glBindVertexArray(axisVAO);
                lineShader.setVec3("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
                glDrawArrays(GL_LINES, 0, 2);
                lineShader.setVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
                glDrawArrays(GL_LINES, 2, 2);
                lineShader.setVec3("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));
                glDrawArrays(GL_LINES, 4, 2);
            }
            glBindVertexArray(0);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ShutdownImGui();
    glfwTerminate();
    return 0;
}
