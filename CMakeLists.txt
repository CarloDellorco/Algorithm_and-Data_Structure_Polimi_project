cmake_minimum_required(VERSION 3.20)

project(Algorithm_project_2022 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_executable(Algorithm_project_2022 Code/main.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Algorithm_project_2022 PRIVATE -Wall -Wextra -O2 -g3)
endif()
