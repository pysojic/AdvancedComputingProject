cmake_minimum_required(VERSION 3.20)
project(orderbook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/permissive- /Zc:preprocessor /O2 /EHsc)
else()
  add_compile_options(-O2 -pthread)
endif()

add_executable(orderbook
  main.cpp
  OrderProcessing.hpp
  Analytics.hpp
)
