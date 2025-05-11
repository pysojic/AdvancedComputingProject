cmake_minimum_required(VERSION 3.10)
project(Part4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")

add_executable(benchmark_ob
  main_ob.cpp
  order_book.h
)
