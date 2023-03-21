cmake_minimum_required(VERSION 3.22)
project(ClientServerCommunication)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_compile_options(-Wall)
set(client client)
set(server server)
set(master master)
add_executable(${client} client.c)
target_link_libraries(${client} ${MPI_LIBRARIES})
add_executable(${server} server.c)
target_link_libraries(${server} ${MPI_LIBRARIES})
add_executable(${master} master.c)
target_link_libraries(${master} ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(${client} PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(${server} PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(${master} PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(${client} PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(${server} PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
    set_target_properties(${master} PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
