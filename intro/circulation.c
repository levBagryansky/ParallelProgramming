// The message (int) circulates within processes,
// every of them increases the message by its rank.
// When the message finish the circle the first process
// prints the message.

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int count;
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        int message = 1;
        int next;
        if (count == 1) {
            printf("count = 0, message = 1\n");
            MPI_Finalize();
            return 0;
        } else {
            next = 1;
        }

        int sent = MPI_Send(&message, 1, MPI_INT, next, 12345, MPI_COMM_WORLD);
        if (sent != MPI_SUCCESS) {
            exit(-1);
        }

        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Received %i\n", message);
    } else {
        int message;
        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int next;
        if (rank == count - 1) {
            next = 0;
        } else {
            next = rank + 1;
        }

        message += (rank + 1);
        int sent = MPI_Send(&message, 1, MPI_INT, next, 12345, MPI_COMM_WORLD);
        if (sent != MPI_SUCCESS) {
            printf("did not send :(\n");
            exit(-1);
        }
    }

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}
