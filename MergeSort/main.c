#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

int size = 0;

int * generate_arr();
void print_arr(int *arr, int len);
int compute_loc(int rank, int world);
int * merge(int *arr1, int len1, int *arr2, int len2);
int comp (const int *i, const int *j);

int main(int argc, char **argv) {
    double w_start, w_end;
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
    int *arr;
    if (my_rank == 0) {
        arr = generate_arr();
        //print_arr(arr, size);
        // Send the according part to every processor
        w_start = MPI_Wtime();
        int *message = arr + loc_size;
        //printf("[Processor %i] loc_size = %i\n", my_rank, loc_size);
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
        //free(arr);
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
    }
    //sleep(my_rank * 0.6);
    qsort(buf, loc_size, sizeof(int), (int(*) (const void *, const void *)) comp);
    //printf("[processor %i] my part:\n", my_rank);
    //print_arr(buf, loc_size);
    if (my_rank) {
//        MPI_Send(buf,
//                 loc_size,
//                 MPI_INT,
//                 0,
//                 0,
//                 MPI_COMM_WORLD
//                 );
        int mod;
        int *res;
        int res_size = loc_size;
        for (int base = 2; base < world * 2; base *= 2) {
            mod = my_rank % base;
            if (mod == 0) {
                int src = my_rank + base / 2;
                if (src < world) {
                    int message_len = (loc_size + 1) * base;
                    int *message_from = (int *) malloc(message_len * sizeof(int));
                    MPI_Status status;
                    MPI_Recv(
                            message_from,
                            message_len,
                            MPI_INT,
                            src,
                            MPI_ANY_TAG,
                            MPI_COMM_WORLD,
                            &status
                    );

                    //int message_size = compute_loc(status.MPI_SOURCE, world);
                    MPI_Get_count(&status, MPI_INT, &message_len);
                    res = merge(buf, res_size, message_from, message_len);
                    res_size += message_len;
                    free(buf);
                    buf = res;
                    //printf("[processor %i]: merged from %i\n", my_rank, status.MPI_SOURCE);
                    //print_arr(message_from, message_size);
                    //print_arr(buf, res_size);
                    free(message_from);
                }
            } else if (mod == base / 2) {
                MPI_Send(
                         buf,
                         res_size,
                         MPI_INT,
                         my_rank - base / 2,
                         0,
                         MPI_COMM_WORLD
                );
                //printf("[processor %i]: sent to %i\n", my_rank, my_rank - base / 2);
                break;
            } else {
                break;
            }
        }
    }
    if (my_rank == 0) {
        int *res;
        int res_size = loc_size;
        for (int i = 1; i < world; i *= 2) {
            int message_len = (loc_size + 1) * (i + 1);
            int* message_from = (int *) malloc(message_len * sizeof(int));
            MPI_Status status;
            MPI_Recv
            (
                message_from,
                message_len,
                MPI_INT,
                MPI_ANY_SOURCE,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );

            //int message_size = compute_loc(status.MPI_SOURCE, world);
            MPI_Get_count(&status, MPI_INT, &message_len);
            res = merge(buf, res_size, message_from, message_len);
            //printf("[processor %i]: merged from %i\n", my_rank, status.MPI_SOURCE);
            res_size += message_len;
            free(buf);
            buf = res;
            //printf("merged from %i\n", status.MPI_SOURCE);
            //print_arr(message_from, message_size);
            //print_arr(buf, res_size);
            free(message_from);
        }
        w_end = MPI_Wtime();
        printf("%f", -w_start+w_end);
//        printf("time is %f, res_size = %i\n", -w_start+w_end, res_size);
//        qsort(arr, size, sizeof(int), (int(*) (const void *, const void *)) comp);
//        if (!memcmp(arr, buf, res_size)) {
//            printf("array was sorted right\n");
//        } else {
//            printf("array was not sorted right\n");
//            printf("Expected\n");
//            print_arr(arr, size);
//        }
        free(arr);
    }
    free(buf);
    MPI_Finalize();
    return 0;
}

int * generate_arr() {
    int *res = (int *) malloc(size * sizeof (int));
    for (int i = 0; i < size; i++) {
        res[i] = rand() % 10000;
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

int * merge(int *arr1, int len1, int *arr2, int len2) {
    int *result = (int *) malloc(sizeof(int) * (len1 + len2));
    int i1 = 0;
    int i2 = 0;
    while (i1 + i2 < len1 + len2) {
        int next;
        if (i1 == len1) {
            next = arr2[i2];
            i2++;
        } else if (i2 == len2){
            next = arr1[i1];
            i1++;
        } else if (arr2[i2] < arr1[i1]){
            next = arr2[i2];
            i2++;
        } else {
            next = arr1[i1];
            i1++;
        }
        result[i1 + i2 - 1] = next;
    }
    return result;
}

int comp (const int *i, const int *j)
{
    return *i - *j;
}
