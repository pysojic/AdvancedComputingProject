cmake_minimum_required(VERSION 3.10.0)
project(Project_Part1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Project_Part1 main.cpp)

target_compile_options(Project_Part1 PRIVATE -O0 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic

