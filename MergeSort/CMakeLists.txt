cmake_minimum_required(VERSION 3.2)
project(MergeSort)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_compile_options(-Wall)
add_executable(main main.c)
target_link_libraries(main ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(main PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()