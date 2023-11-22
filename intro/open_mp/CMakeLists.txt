cmake_minimum_required(VERSION 3.26)
project(open_mp_intro)

add_executable(HelloWorld HelloWorld.cpp)
add_executable(Sum Sum.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(HelloWorld PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(Sum PUBLIC OpenMP::OpenMP_CXX)
endif()

