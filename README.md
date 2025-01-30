# **Project Setup Guide**

This guide explains how to clone the repository, set up dependencies, and build the project on **Linux** and **Windows**.

---

## **Clone the Repository**

1. Open a terminal or command prompt.
2. Run the following command to clone the repository:
   ```bash
   git clone https://github.com/ishangautam7/graphics.git
   cd graphics
   ```

---

## **Linux Setup**

### **1. Install Dependencies**
Run the following command to install the required libraries:
```bash
sudo pacman -S base-devel cmake glfw-x11 glm assimp libx11 libxi libxrandr libxxf86vm
```

### **2. Build the Project**
```bash
mkdir build
cd build
cmake ..
make
```

### **3. Run the Executable**
After building, run the program:
```bash
./ComputerGraphics
```

---

## **Windows Setup**

### **1. Install Dependencies**
- **GLFW**: Download and install from [GLFW's website](https://www.glfw.org/download.html).
- **Assimp**:
  - Download the latest version from [Assimp's GitHub](https://github.com/assimp/assimp).
  - Build Assimp using CMake and Visual Studio (follow the instructions in the Assimp repository).
  - Alternatively, install Assimp using `vcpkg`:
    ```cmd
    vcpkg install assimp
    ```
- **OpenGL**: Ensure your graphics drivers are up to date.

### **2. Build the Project**
1. Open a terminal in the project directory.
2. Run the following commands:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

### **3. Run the Executable**
Navigate to the `build` directory and run `ComputerGraphics.exe`.

---

## **Folder Structure**
- `includes/`: Header files.
- `src/`: Source code.
- `build/`: Build directory (created during setup).

---

## **Notes**
- Ensure all dependencies are installed before building.
- For issues, check the error logs and verify paths to libraries.
- If using `vcpkg` on Windows, pass the `vcpkg` toolchain file to CMake:
  ```bash
  cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
  ```

---

Let me know if you need further adjustments!
