# Project Setup Guide
This guide explains how to clone the repository and build the project.

---
## Clone the Repository
1. Open a terminal or command prompt.
2. Run the following command to clone the repository:
   ```bash
   git clone -b 3dengine-working https://github.com/ishangautam7/3DEngine.git
   cd 3DEngine
   ```

---
## External Libraries
Clone these repositories inside the `graphics/Libraries/includes` directory:

### ImGui
```bash
git clone https://github.com/ocornut/imgui.git
```

### STB
```bash
git clone https://github.com/nothings/stb.git
```

---
## Build Instructions
```bash
# Empty the build directory first if build directory is present
mkdir build
cd build
cmake ..
cmake --build .
```

---
## Path Configuration
The project uses several resource paths that may need to be updated based on your project location:

### In `main.cpp`:
Update these paths according to your project location:
```cpp
Shader shader("D:/3DEngine/shaders/vertex.glsl", "D:/3DEngine/shaders/fragment.glsl");
Model model("D:/3DEngine/assests/colgate.glb");
```

Replace `D:/3DEngine` with your actual project path. For example:
- Linux: `/home/username/3DEngine`
- Windows: `C:/Users/username/3DEngine`

---
## Complete Folder Structure
```
graphics/
│
├── CMakeLists.txt
│
├── Libraries/
│   ├── includes/
│   │   ├── stb/
│   │   │   └── stb_image.h
│   │   ├── imgui/
│   │   │   ├── imgui.h
│   │   │   ├── imgui.cpp
│   │   │   ├── imgui_demo.cpp
│   │   │   ├── imgui_draw.cpp
│   │   │   ├── imgui_tables.cpp
│   │   │   ├── imgui_widgets.cpp
│   │   │   └── backends/
│   │   │       ├── imgui_impl_glfw.cpp
│   │   │       ├── imgui_impl_glfw.h
│   │   │       ├── imgui_impl_opengl3.cpp
│   │   │       └── imgui_impl_opengl3.h
│   │   ├── glad/
│   │   │   ├── glad.h
│   │   │   └── (Other GLAD headers)
│   │   ├── glfw/
│   │   │   └── (GLFW headers)
│   │   ├── glm/
│   │   │   └── (GLM headers)
│   │   ├── assimp/
│   │   │   └── (Assimp headers)
│   │   └── KHR/
│           └── khrplatform.h
│
├── lib/
│   ├── Release/
│   └── Debug/
│
├── src/
│   ├── camera.cpp
│   ├── camera.h
│   ├── glad.c
│   ├── globals.cpp
│   ├── globals.h
│   ├── imgui_impl.cpp
│   ├── imgui_impl.h
│   ├── input.cpp
│   ├── input.h
│   ├── main.cpp
│   ├── mesh.cpp
│   ├── mesh.h
│   ├── model.cpp
│   ├── model.h
│   ├── shader.cpp
│   ├── shader.h
│   ├── structures.h
│   └── texture.cpp
│
├── shaders/
│   └── (GLSL shader files)
│
├── assets/
│   └── (3D model files)
```

---
## Notes
- Ensure all dependencies are installed before building.
- Check error logs and verify library paths if you encounter build issues.
- Double-check the resource paths in your code match your actual project structure.
