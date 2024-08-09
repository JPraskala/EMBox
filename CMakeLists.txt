cmake_minimum_required(VERSION 3.28)
project(EMBox C)

set(CMAKE_C_STANDARD 23)

# Find the OpenGL package
find_package(OpenGL REQUIRED)

# Include the OpenGL headers
include_directories(${OPENGL_INCLUDE_DIRS})

# Add your executable
add_executable(EMBox
        main.c
        video_processors/vdp1.c
        video_processors/vdp1.h
        video_processors/vdp2.c
        video_processors/vdp2.h
)

# Link the OpenGL libraries to your executable
target_link_libraries(EMBox ${OPENGL_LIBRARIES})
