cmake_minimum_required(VERSION 3.28)
project(EMBox C)

set(CMAKE_C_STANDARD 23)

add_executable(EMBox main.c
        audio/headers/dac.h
        audio/headers/motorola.h
        audio/headers/scsp.h
        audio/headers/ram.h
        audio/c_files/dac.c
        audio/c_files/motorola.c
        audio/c_files/ram.c
        audio/c_files/scsp.c)
