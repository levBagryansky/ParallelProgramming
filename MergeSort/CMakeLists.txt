cmake_minimum_required(VERSION 3.2)
project(MergeSort C)

set(CMAKE_C_STANDARD 99)
find_package(MPI REQUIRED)
add_compile_options(-Wall)

add_executable(main main.c)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(main ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(main PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if (MPI_LINK_FLAGS)
    set_target_properties(main PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif ()