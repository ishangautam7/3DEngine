#include <glad/glad.h> // Include glad.h FIRST
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h> // Include GLFW after glad.h
#include <iostream>
#include "model.h"
#include "shader.h"
#include <filesystem>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Model Viewer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize Glad" << std::endl;
        return -1;
    }

    // Set up viewport
    glViewport(0, 0, 800, 600);

    // Enable depth testing (important for 3D rendering)
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    Shader shader("/home/ishan/graphics/shaders/vertex.glsl", "/home/ishan/graphics/shaders/fragment.glsl");

    // Projection matrix (Perspective projection)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // View matrix (Camera position)
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 10.0f),  // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Look-at point (center)
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
    );

    // Load model
    Model model("/home/ishan/graphics/assests/h2.glb");
    if (model.isEmpty()) {
        std::cerr << "Model failed to load!" << std::endl;
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen (both color and depth buffers)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Use shader before setting uniforms
        shader.use();


        // Pass transformation matrices to shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Apply model transformation
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); // Adjust scale if needed
        shader.setMat4("model", modelMatrix);

        // Render the model
        model.draw(shader);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}
