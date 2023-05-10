#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    double start = MPI_Wtime();
    int count = 500;
    if (myrank == 1){
        int message = 1234;
        for (int i = 0; i < count; ++i) {
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            //printf("sent\n");
        }
    } else if (myrank == 0){
        int message;
        MPI_Status status;
        for (int i = 0; i < count; ++i) {
            MPI_Recv(&message, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //printf("recn\n");
        }
        double finish = MPI_Wtime();
        printf("average %.30lf\n", (finish - start) / count);
    }
    // Finalize the MPI environment.
    MPI_Finalize();
}
