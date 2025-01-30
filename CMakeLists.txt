cmake_minimum_required(VERSION 3.31.2)
project(ComputerGraphics)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Libraries/includes
)

# Add executable with glad.c
add_executable(ComputerGraphics src/main.cpp src/glad.c src/model.cpp src/mesh.cpp src/shader.cpp)

# Platform-specific configurations
if(WIN32)
    message("Configuring for Windows")

    find_package(assimp REQUIRED)

    include_directories(${CMAKE_SOURCE_DIR}/Libraries/includes/glad)
    
    target_link_libraries(ComputerGraphics glfw3 opengl32 assimp-vc142-mt)

elseif(UNIX)
    message("Configuring for Linux")

    find_package(assimp REQUIRED)

    target_link_libraries(ComputerGraphics GL GLU glfw dl X11 Xxf86vm Xrandr pthread Xi ${ASSIMP_LIBRARIES})

    include_directories(${ASSIMP_INCLUDE_DIRS})
endif()

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)