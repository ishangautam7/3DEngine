#include <GLFW/glfw3.h>
#include <iostream>

float xPos = 0.0f, yPos = 0.0f;
float speed = 0.01f;  

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {  
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)  xPos -= speed;
        if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) xPos += speed;
        if (key == GLFW_KEY_UP || key == GLFW_KEY_W)    yPos += speed;
        if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)  yPos -= speed;

        xPos = std::max(-1.0f, std::min(xPos, 1.0f)); 
        yPos = std::max(-1.0f, std::min(yPos, 1.0f)); 
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }

    int screenWidth = 1000;
    int screenHeight = 800;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Graphics Project", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);  // Set key callback function

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a moving square (or point)
        glBegin(GL_QUADS);
        glVertex2f(xPos - 0.05f, yPos - 0.05f);
        glVertex2f(xPos + 0.05f, yPos - 0.05f);
        glVertex2f(xPos + 0.05f, yPos + 0.05f);
        glVertex2f(xPos - 0.05f, yPos + 0.05f);
        glEnd();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
