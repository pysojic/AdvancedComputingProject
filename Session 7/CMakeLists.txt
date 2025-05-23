# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)       # 3.20+ ships with VS 2022
project(orderbook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Extra warnings / optimisation flags for MSVC; harmless on GCC/Clang
if (MSVC)
    add_compile_options(/permissive- /Zc:preprocessor /O2 /EHsc)
else()
    add_compile_options(-O2 -pthread)
endif()

add_executable(orderbook main.cpp)         # headers are pulled in by include‑directives
