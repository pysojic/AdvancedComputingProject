cmake_minimum_required(VERSION 3.10.0)
project(Final_Project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.65 REQUIRED COMPONENTS filesystem system)

include_directories(include)

add_executable(Final_Project_Server src/main_server.cpp)
add_executable(Final_Project_Client src/main_client.cpp)

target_compile_options(Final_Project_Server PRIVATE -O2 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic
target_compile_options(Final_Project_Client PRIVATE -O2 -march=native) # Use for debug: -O0 -g -Wall -Wextra -Wpedantic

target_include_directories(Final_Project_Client PRIVATE "/Users/py/Documents/C++ libs/compile-time-regular-expressions/single-header")
target_link_libraries(Final_Project_Server PRIVATE Boost::filesystem Boost::system)



