#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

extern bool mouseEnabled;
extern bool rightMousePressed;
extern bool leftMousePressed;
extern bool wireframeMode;
extern bool usePerspective;
extern float orthoSize;
extern glm::vec3 modelPosition;
extern glm::vec3 modelRotation;
extern float modelScale;

#endif
