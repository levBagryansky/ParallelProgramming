cmake_minimum_required(VERSION 3.2)
project(Write2File)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_compile_options(-Wall)
add_executable(write_process write_process.c)
target_link_libraries(write_process ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(write_process PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(write_process PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
