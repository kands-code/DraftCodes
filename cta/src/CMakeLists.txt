cmake_minimum_required(VERSION 3.25)

# use clang for defualt compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 17)

project(cta C)

add_subdirectory("cta")

add_executable("cta-exe" "main.c")

target_link_libraries("cta-exe" ${PROJECT_NAME})
