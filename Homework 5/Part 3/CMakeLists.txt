cmake_minimum_required(VERSION 3.10)
project(Part3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /arch:AVX2")

add_executable(benchmark_ts
  main_ts.cpp
  time_series.h
)
