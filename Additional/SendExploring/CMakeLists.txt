cmake_minimum_required(VERSION 2.7)
project(Sends)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_compile_options(-Wall)
add_executable(default default_send.c)
target_link_libraries(default ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(default PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(default PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()