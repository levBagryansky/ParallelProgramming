#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

int size = 0;

int * generate_arr() {
    int *res = (int *) malloc(size * sizeof (int));
    for (int i = 0; i < size; i++) {
        res[i] = rand();
    }
    return res;
}

void print_arr(int *arr, int len) {
    for (int i = 0; i < len; i++) {
        printf("%i ", arr[i]);
    }
    printf("\n");
}

int compute_loc(int rank, int world) {
    int loc_size = size / world;
    if (size % world > (world - 1 - rank)) {
        loc_size++;
    }
    return loc_size;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int world;
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    srand(time(0));
    if(argc != 2) {
        printf("Expected size argument\n");
        exit(-1);
    }
    size = strtol(argv[1], NULL, 10);
    int loc_size = compute_loc(my_rank, world);
    int *buf = (int *) malloc(loc_size * sizeof(int));
    if (my_rank == 0) {
        int *arr = generate_arr();
        print_arr(arr, size);
        // Send the according part to every processor
        int *message = arr + loc_size;
        printf("[Processor %i] loc_size = %i\n", my_rank, loc_size);
        for (int i = 1; i < world; i++) {
            int count = compute_loc(i, world);
            MPI_Send(
                message,
                count,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD);
            message += count;
        }
        memcpy(buf, arr, loc_size * sizeof(int));
        printf("[processor %i] my part:\n", my_rank);
        print_arr(buf, loc_size);
        free(arr);
    } else {
        MPI_Status status;
        MPI_Recv(
            buf,
            loc_size,
            MPI_INT,
            0,
            MPI_ANY_TAG,
            MPI_COMM_WORLD,
            &status
                );
        sleep(my_rank * 0.6);
        printf("[processor %i] my part:\n", my_rank);
        print_arr(buf, loc_size);
        free(buf);
    }

    MPI_Finalize();
    return 0;
}
