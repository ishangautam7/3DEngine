// Fragment Shader (e.g., fragment.glsl)
#version 450 core

out vec4 FragColor;  // Output color

in vec3 FragPos;  // Interpolated position
in vec3 Normal;   // Interpolated normal

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
}

