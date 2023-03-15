#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    if (argc != 1) {
        printf("Wrong number of arguments\n");
        exit(-1);
    }


    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Group group_world = 0;
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);
    printf("group_world %i\n", group_world);

    MPI_Group excluded = 0;
    int ranks[1] = {1};
    MPI_Group_excl(group_world, 1, ranks, &excluded);
    printf("excluded = %i\n", excluded);

    //Create a new communicator from group
    MPI_Comm new_comm;
    MPI_Comm_create(MPI_COMM_WORLD, excluded, &new_comm);
    printf("new_comm = %i\n", new_comm);


    return 0;
}
