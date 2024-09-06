cmake_minimum_required(VERSION 3.28)
project(EMBox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Find GLFW
find_package(glfw3 REQUIRED)

# Glad setup
set(GLAD_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/glad/src")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/glad/include")
add_library(glad "${GLAD_SOURCE_DIR}/glad.c")
target_include_directories(glad PUBLIC "${GLAD_INCLUDE_DIR}")

# Add the executable
add_executable(EMBox
        main.c
        video_processors/vdp1.c
        video_processors/vdp1.h
        video_processors/vdp2.c
        video_processors/vdp2.h
        video_processors/graphicsManager.c
)

# Link libraries
target_link_libraries(EMBox glad glfw ${OPENGL_LIBRARIES})
