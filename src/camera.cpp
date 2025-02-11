#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
// Global camera instance
Camera camera;
// Constructor initializing camera properties
Camera::Camera() : 
    yaw(-90.0f), pitch(0.0f), lastX(400.0f), lastY(300.0f),
    firstMouse(true), cameraDistance(20.0f), fov(45.0f),
    cameraPos(0.0f, 0.0f, 20.0f), cameraFront(0.0f, 0.0f, -1.0f),
    cameraUp(0.0f, 1.0f, 0.0f), cameraSpeed(0.05f) {}
// Update camera position based on spherical coordinates
void Camera::updateCameraVectors() {
    glm::vec3 direction;
    // Calculate new camera position using spherical coordinates
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraPos = direction * cameraDistance;
}
// Get the view matrix for the camera
glm::mat4 Camera::getViewMatrix() const {
    // Create a look-at matrix for the camera
    return glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
}
