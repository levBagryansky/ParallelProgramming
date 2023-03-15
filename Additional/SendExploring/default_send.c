#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MESSAGE 12345
#define ARR_LITTLE_COUNT 10000
#define ARR_BIG_COUNT 100000
#define SLEEP 3

void DefaultSendIntTest(int world_rank);
void DefaultSendArrayTest(int world_rank, int count);
void SsendArrayTest(int world_rank, int count);
void BsendArrayTest(int world_rank, int count);
void RsendArrayTest(int world_rank, int count);

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

    DefaultSendArrayTest(world_rank, ARR_LITTLE_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    DefaultSendArrayTest(world_rank, ARR_BIG_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    SsendArrayTest(world_rank, ARR_LITTLE_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    SsendArrayTest(world_rank, ARR_BIG_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    //BsendArrayTest(world_rank, 1);
    MPI_Barrier(MPI_COMM_WORLD);

    //BsendArrayTest(world_rank, ARR_BIG_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    RsendArrayTest(world_rank, ARR_LITTLE_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    RsendArrayTest(world_rank, ARR_BIG_COUNT);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;

}

void DefaultSendIntTest(int world_rank) {
    if (world_rank == 0) {
        // Receive
        int message = 0;
        MPI_Status status;
        //printf("Receiver goes to sleep..\n");
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
        printf("MPI_Send send INT test:\n");
        int message = MESSAGE;
        //printf("Sender starts to measure\n");
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
        fflush(stdout);
        printf("MPI_Send returns control almost immediately\n\n");
    }

}

void DefaultSendArrayTest(int world_rank, int count){
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        // Receive
        printf("MPI_Send send ARRAY (with count %i) test:\n", count);
        int *message = (int *) calloc(count, sizeof (int));
        MPI_Status status;
        //printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                message,
                count,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        free(message);
    } else if (world_rank == 1) {
        // Send
        int *message = (int *) calloc(count, sizeof (int));
        //printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Send(
                message,
                count,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Sender waited for %f seconds\n", seconds);
        fflush(stdout);
        free(message);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 1) {
        printf("MPI_Send returns control not so fast\n\n");
        fflush(stdout);
        //sleep(0.3);
    }
    MPI_Barrier(MPI_COMM_WORLD);

}

void SsendArrayTest(int world_rank, int count) {
    if (world_rank == 0) {
        // Receive
        printf("MPI_Ssend send ARRAY with %i test:\n", count);
        int *message = (int *) calloc(count, sizeof (int));
        MPI_Status status;
        //printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                message,
                count,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        free(message);
    } else if (world_rank == 1) {
        // Send
        int *message = (int *) calloc(count, sizeof (int));
        //printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Ssend(
                message,
                count,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Ssender waited for %f seconds\n", seconds);
        fflush(stdout);
        free(message);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 1){
        printf("MPI_Ssend returns control not so fast\n\n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void BsendArrayTest(int world_rank, int count) {
    if (world_rank == 0) {
        // Receive
        printf("MPI_Bsend send ARRAY with %i test:\n", count);
        int *message = (int *) calloc(count, sizeof (int));
        int mess;
        MPI_Status status;
        //printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                &mess,
                count,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        free(message);
    } else if (world_rank == 1) {
        // Send
        int *message = (int *) calloc(count, sizeof (int));
        int mess = 1;
        //printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Bsend(
                &mess,
                count,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Sender waited for %f seconds\n", seconds);
        fflush(stdout);
        free(message);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 1){
        printf("MPI_Bsend returns control not so fast\n\n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void RsendArrayTest(int world_rank, int count) {
    if (world_rank == 0) {
        // Receive
        printf("MPI_Rsend send ARRAY with %i test:\n", count);
        int *message = (int *) calloc(count, sizeof (int));
        MPI_Status status;
        //printf("Receiver goes to sleep..\n");
        sleep(SLEEP);
        MPI_Recv(
                message,
                count,
                MPI_INT,
                1,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
        );
        free(message);
    } else if (world_rank == 1) {
        // Send
        int *message = (int *) calloc(count, sizeof (int));
        //printf("Sender starts to measure\n");
        clock_t start = clock();
        MPI_Rsend(
                message,
                count,
                MPI_INT,
                0,
                0,
                MPI_COMM_WORLD
        );
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("RSender waited for %f seconds\n", seconds);
        fflush(stdout);
        free(message);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 1){
        printf("MPI_Rsend returns control not so fast\n\n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
