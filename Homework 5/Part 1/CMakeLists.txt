cmake_minimum_required(VERSION 3.10)
project(Part1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")

add_executable(benchmark
  main.cpp
  robinhood_hash_table.h
)
