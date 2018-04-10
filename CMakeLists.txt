# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.10)

project(cv1)

set(CMAKE_CXX_STANDARD 11)

add_executable(cv1 main.cpp TaskInstance.cpp TaskInstance.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")