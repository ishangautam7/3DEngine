#include "globals.h"
// Flag to switch between perspective and orthographic projection
// true: use perspective projection, false: use orthographic projection
bool usePerspective = true;
// Size of the orthographic view frustum
// Controls how much of the scene is visible when using orthographic 
float orthoSize = 5.0f;
// 3D vector representing the model's position in world space
// x: left/right, y: up/down, z: forward/backward
glm::vec3 modelPosition(0.0f);
// 3D vector representing the model's rotation in degrees
// x: pitch (rotation around x-axis)
// y: yaw (rotation around y-axis)
// z: roll (rotation around z-axis)
glm::vec3 modelRotation(0.0f);
// Uniform scale factor for the model
// 1.0f means original size, <1.0f shrinks, >1.0f enlarges
float modelScale = 1.0f;
// Flag to toggle wireframe rendering mode
// true: render as wireframe, false: render as solid
bool wireframeMode = false;
