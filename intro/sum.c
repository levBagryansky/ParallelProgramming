#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        printf("Wrong number of arguments\n");
        exit(-1);
    }

    int N = atoi(argv[1]);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        // Sum it
        printf("my rank == 0\n");
        double sum = 0;
        double buf = 0;
        for (int i = 0; i < 2; ++i) {
            printf("wait for next term\n");
            MPI_Recv(&buf, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += buf;
        }
        printf("sum  = %lf\n", sum);
    } else {
        // Compute a term
        double term = 1.0 / rank;
        printf("Sent %lf\n", term);
        int sent = MPI_Send(&term,
                             1,
                             MPI_DOUBLE,
                             0,
                             5,
                             MPI_COMM_WORLD);
        assert(sent == MPI_SUCCESS);

    }
    MPI_Finalize();
    return 0;

    // Finalize the MPI environment.
}