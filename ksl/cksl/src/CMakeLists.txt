cmake_minimum_required(VERSION 3.25)

# use clang for defualt compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 17)

project(cksl C)

add_subdirectory("cksl")

add_executable(ckslp "main.c")

target_link_libraries(ckslp ${PROJECT_NAME})
