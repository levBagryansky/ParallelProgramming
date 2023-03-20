cmake_minimum_required(VERSION 2.7)
project(ITK_MPI)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_compile_options(-Wall)
add_executable(exclude_communicator exclude_communicator.c)
target_link_libraries(exclude_communicator ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(exclude_communicator PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(exclude_communicator PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
