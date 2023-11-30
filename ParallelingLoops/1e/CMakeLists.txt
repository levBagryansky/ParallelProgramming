cmake_minimum_required(VERSION 3.26)
project(1e C)

find_package(MPI REQUIRED)
set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall)
add_executable(Sequential sequential.c)

add_executable(Parallel parallel.c)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(Parallel
        PRIVATE m
        ${MPI_LIBRARIES}
        )
if(MPI_COMPILE_FLAGS)
    set_target_properties(Parallel PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(Sequential PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(Parallel PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(Sequential PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

target_link_libraries(Sequential PRIVATE m ${MPI_LIBRARIES})
