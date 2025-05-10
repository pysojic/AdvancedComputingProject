cmake_minimum_required(VERSION 3.10.0)
project(Project_Part5 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Project_Part5_1 robin_hood_hashing.cpp)
add_executable(Project_Part5_2 time_series.cpp)
add_executable(Project_Part5_3 priority_queue.cpp)

target_compile_options(Project_Part5_1 PRIVATE -O0 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic
target_compile_options(Project_Part5_2 PRIVATE -O0 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic
target_compile_options(Project_Part5_3 PRIVATE -O0 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic

