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
    //printf("group_world %i\n", group_world);

    MPI_Group excluded = 0;
    int ranks[1] = {1};
    MPI_Group_excl(group_world, 1, ranks, &excluded);
    //printf("excluded = %i\n", excluded);

    //Create a new communicator from group
    MPI_Comm new_comm;
    MPI_Comm_create(MPI_COMM_WORLD, excluded, &new_comm);

    if (new_comm == MPI_COMM_NULL) {
        printf("new_comm == MPI_COMM_NULL, exit\n");
        MPI_Finalize();
        return 0;
    }
    int local_rank;
    MPI_Comm_rank(new_comm, &local_rank);
    int local_size = 0;
    MPI_Comm_size(new_comm, &local_size);
    assert(world_size - local_size == 1);
    printf("Hello from process %i out of %i\n", local_rank, local_size);

    int source = local_rank + 1;
    int result = 0;
    int root = 0;

    MPI_Reduce(&source, &result, 1, MPI_INT, MPI_SUM, root, new_comm);
    if (local_rank == root) {
        printf("result = %i\n", result);
    }
    MPI_Finalize();
    return 0;
}
