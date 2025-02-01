cmake_minimum_required(VERSION 3.31.2)
project(ComputerGraphics)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for better optimization
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2")
    # Fix MSVCRT conflict
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Include common directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Libraries/includes
    ${CMAKE_SOURCE_DIR}/Libraries/includes/stb
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Libraries/includes/imgui
    ${CMAKE_SOURCE_DIR}/Libraries/includes/imgui/backends
)

# Build ImGui as static library
add_library(imgui STATIC
    Libraries/includes/imgui/imgui.cpp
    Libraries/includes/imgui/imgui_demo.cpp
    Libraries/includes/imgui/imgui_draw.cpp
    Libraries/includes/imgui/imgui_tables.cpp
    Libraries/includes/imgui/imgui_widgets.cpp
    Libraries/includes/imgui/backends/imgui_impl_glfw.cpp
    Libraries/includes/imgui/backends/imgui_impl_opengl3.cpp
   
)

# Add executable with source files
add_executable(ComputerGraphics 
    src/main.cpp 
    src/glad.c 
    src/model.cpp 
    src/mesh.cpp 
    src/shader.cpp
    src/texture.cpp
    src/camera.cpp
    src/input.cpp
    src/imgui_impl.cpp
    src/globals.cpp  # Add this line
)



# Platform-specific configurations
if(WIN32)
    message("Configuring for Windows")

    # Assimp setup for Windows
    include_directories(${CMAKE_SOURCE_DIR}/Libraries/includes/assimp)
    target_link_directories(ComputerGraphics PRIVATE 
        ${CMAKE_SOURCE_DIR}/Libraries/lib/Release
        ${CMAKE_SOURCE_DIR}/Libraries/lib
    )

    target_link_libraries(ComputerGraphics 
        glfw3 
        opengl32 
        assimp-vc142-mt
        imgui
    )

elseif(UNIX)
    message("Configuring for Linux")

    # Assimp and other Linux dependencies
    find_package(assimp REQUIRED)
    find_package(OpenGL REQUIRED)
    include_directories(${ASSIMP_INCLUDE_DIRS})

    target_link_libraries(ComputerGraphics 
        ${OPENGL_LIBRARIES}
        glfw
        dl
        X11
        Xxf86vm
        Xrandr
        pthread
        Xi
        ${ASSIMP_LIBRARIES}
        imgui
    )
endif()

# Enable debugging symbols in Debug mode
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Set output directory relative to project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Copy shader files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
