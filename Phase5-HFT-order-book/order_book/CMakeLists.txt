cmake_minimum_required(VERSION 3.10)
project(benchmark_orderbook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(benchmark_orderbook src/main.cpp)