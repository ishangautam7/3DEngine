#include "input.h"
#include "camera.h"
#include "imgui_impl.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "globals.h"

bool mouseEnabled = false;
bool rightMousePressed = false;
bool leftMousePressed = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            mouseEnabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
            mouseEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMousePressed = (action == GLFW_PRESS);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (!mouseEnabled) return;

    if (camera.firstMouse) {
        camera.lastX = static_cast<float>(xpos);
        camera.lastY = static_cast<float>(ypos);
        camera.firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - camera.lastX;
    float yoffset = camera.lastY - static_cast<float>(ypos);
    camera.lastX = static_cast<float>(xpos);
    camera.lastY = static_cast<float>(ypos);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (rightMousePressed) {
        camera.yaw += xoffset;
        camera.pitch += yoffset;
        camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
    } else if (leftMousePressed) {
        modelRotation.y += xoffset;
        modelRotation.x += yoffset;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    if (ImGui::GetIO().WantCaptureMouse) return;

    float zoomSpeed = camera.cameraDistance * 0.1f;
    camera.cameraDistance -= static_cast<float>(yoffset) * zoomSpeed;
    camera.cameraDistance = glm::clamp(camera.cameraDistance, 1.0f, 1000.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard) return;

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        wireframeMode = !wireframeMode;
    }
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

    float currentSpeed = camera.cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        currentSpeed *= 2.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        modelPosition.z -= currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        modelPosition.z += currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        modelPosition.x -= currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        modelPosition.x += currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        modelPosition.y += currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        modelPosition.y -= currentSpeed;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        modelScale = glm::max(0.1f, modelScale - currentSpeed);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        modelScale += currentSpeed;
}
