# cmake_minimum_required(VERSION <specify CMake version here>)
project(cv1)

set(CMAKE_CXX_STANDARD 11)

add_executable(cv1 main.cpp TaskInstance.cpp TaskInstance.h)
