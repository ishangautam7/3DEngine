#include "globals.h"

// Perspective vs orthographic projection
bool usePerspective = true;
// Orthographic view size
float orthoSize = 5.0f;
// Model transform state
glm::vec3 modelPosition(0.0f);
glm::vec3 backgroundColor(0.510f, 0.620f, 0.620f);
glm::vec3 modelRotation(0.0f);
float modelScale = 1.0f;
// Wireframe rendering toggle
bool wireframeMode = false;
