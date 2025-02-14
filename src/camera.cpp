#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
Camera camera;
Camera::Camera() : 
    yaw(-90.0f), pitch(0.0f), lastX(400.0f), lastY(300.0f),
    firstMouse(true), cameraDistance(20.0f), fov(45.0f),
    cameraPos(0.0f, 0.0f, 20.0f), cameraFront(0.0f, 0.0f, -1.0f),
    cameraUp(0.0f, 1.0f, 0.0f), cameraSpeed(0.05f) {}
    void Camera::updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        cameraPos = -direction * cameraDistance; // Inverse direction for orbital
        cameraFront = glm::normalize(direction);
    }
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
}
