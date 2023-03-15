#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MESSAGE 12345
#define ARR_COUNT 100000
#define SLEEP 3

void DefaultSendIntTest(int world_rank);
void DefaultSendArrayTest(int world_rank);
void SendArrayTest(int world_rank);

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    if (argc != 1) {
        printf("Wrong number of arguments\n");
        exit(-1);
    }

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size <= 1) {
        printf("Need at least 2 processes\n");
    }

    DefaultSendIntTest(world_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    DefaultSendArrayTest(world_rank);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;

}

void DefaultSendIntTest(int world_rank) {
    if (world_rank == 0) {
        // Receive
        printf("MPI_Send send INT test:\n");
        int message = 0;
        MPI_Status status;
        printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                &message,
                1,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        assert(message == MESSAGE);
        //printf("message = %i\n", message);
    } else if (world_rank == 1) {
        // Send
        int message = MESSAGE;
        printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Send(
                &message,
                1,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Sender waited for %f seconds\n", seconds);
    }

    if (world_rank == 1){
        printf("MPI_Send returns control almost immediately\n\n");
    }
}

void DefaultSendArrayTest(int world_rank){
    if (world_rank == 0) {
        // Receive
        printf("MPI_Send send ARRAY test:\n");
        int *message = (int *) calloc(ARR_COUNT, sizeof (int));
        MPI_Status status;
        printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                message,
                ARR_COUNT,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        free(message);
    } else if (world_rank == 1) {
        // Send
        int *message = (int *) calloc(ARR_COUNT, sizeof (int));
        printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Send(
                message,
                ARR_COUNT,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Sender waited for %f seconds\n", seconds);
        free(message);
    }

    if (world_rank == 1){
        printf("MPI_Send returns control not so fast\n\n");
    }
}
