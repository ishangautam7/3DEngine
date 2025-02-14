#include "input.h"
#include "camera.h"
#include "imgui_impl.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "globals.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        wireframeMode = !wireframeMode;
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        modelPosition = glm::vec3(0.0f);
        modelRotation = glm::vec3(0.0f);
        modelScale = 1.0f;
        camera.cameraDistance = 20.0f;
        camera.yaw = -90.0f;
        camera.pitch = 0.0f;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 
        camera.cameraSpeed * 2.0f : camera.cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) modelPosition.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) modelPosition.z += speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) modelPosition.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) modelPosition.x += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) modelPosition.y += speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) modelPosition.y -= speed;
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        modelScale = glm::max(0.1f, modelScale - speed);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        modelScale += speed;
}
