cmake_minimum_required(VERSION 3.26)
project(Integral)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++11 -Wall)
add_executable(Integral main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Integral PRIVATE Threads::Threads)