#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Calculate sum of harmonic series.

double localSum(int rank, int comm_size, int N);

int main(int argc, char* argv[])
{
    int N = (int) strtol(argv[1], NULL, 10);

    MPI_Init(&argc, &argv);

    // Get my rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get number of processes
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Create the window
    double window_buffer = 0;
    MPI_Win window;
    MPI_Win_create(&window_buffer, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &window);
    MPI_Win_fence(0, window);

    double local_sum = localSum(rank, world_size, N);

    // Push my value into the first integer in MPI process 0 window
    MPI_Accumulate(&local_sum, 1, MPI_DOUBLE, 0, 0, 1, MPI_DOUBLE, MPI_SUM, window);

    // Wait for the MPI_Accumulate issued to complete before going any further
    MPI_Win_fence(0, window);

    if(rank == 0)
    {
        printf("[MPI process 0] Sum = %lf.\n", window_buffer);
    }

    // Destroy the window
    MPI_Win_free(&window);

    MPI_Finalize();

    return EXIT_SUCCESS;
}

double localSum(int rank, int comm_size, int N) {
    double result = 0.0;
    int mod = rank % (comm_size);
    for (int i = 0; 1 + mod + i * (comm_size) <= N; ++i) {
        double term = 1.0 / (1 + mod + i * (comm_size));
        result += term;
    }

    return result;
}