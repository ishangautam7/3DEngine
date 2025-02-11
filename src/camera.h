#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    void updateCameraVectors();
    glm::mat4 getViewMatrix() const;
    
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;
    float cameraDistance;
    float fov;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
};

extern Camera camera;

#endif
