cmake_minimum_required(VERSION 3.28)
project(EMBox C)

set(CMAKE_C_STANDARD 23)

add_executable(EMBox main.c
        smpc.c
        smpc.h)
