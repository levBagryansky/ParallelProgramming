cmake_minimum_required(VERSION 3.26)
project(3z CXX)

set(CMAKE_C_STANDARD 99)
find_package(OpenMP REQUIRED)
add_compile_options(-Wall)
add_executable(Sequential sequential.cpp)

add_executable(Parallel parallel.cpp)
target_link_libraries(Parallel
        PRIVATE m
        OpenMP::OpenMP_CXX
        )


target_link_libraries(Sequential PRIVATE m OpenMP::OpenMP_CXX)
