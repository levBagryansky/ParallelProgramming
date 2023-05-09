cmake_minimum_required(VERSION 3.26)
project(threads)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++11 -pthread -Wall)
add_executable(Hello hello.cpp)
add_executable(Sum sum.cpp)
add_executable(SequentialAccess sequential_access.cpp)
