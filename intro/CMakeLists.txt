cmake_minimum_required(VERSION 3.22)
project(MPI_Put C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
add_compile_options(-Wall)

add_executable(measure_time measure_time.c)
target_link_libraries(measure_time ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
    set_target_properties(measure_time PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(measure_time PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
