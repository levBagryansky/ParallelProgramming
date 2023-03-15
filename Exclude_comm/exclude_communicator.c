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

    MPI_Group group_world;
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);

    MPI_Finalize();
    return 0;
}
