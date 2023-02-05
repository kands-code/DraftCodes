cmake_minimum_required(VERSION 3.25)

# use clang for defualt compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 17)

project(cmatrix C)

add_subdirectory("cmatrix")

add_executable("${PROJECT_NAME}-exe" "main.c")

target_link_libraries("${PROJECT_NAME}-exe" ${PROJECT_NAME} m)