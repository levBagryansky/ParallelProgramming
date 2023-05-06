cmake_minimum_required(VERSION 3.26)
project(TransportEquation C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-lm -Wall)
add_executable(Sequential sequential.c)
target_link_libraries(Sequential PRIVATE m)
