#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/**
 * Macros. Appends rank and line before the message.
 */
#define DB_PRINT(fmt, ...) ; //printf("[MPI process %d, line %d]: " fmt, my_rank, __LINE__, ##__VA_ARGS__)

#define SIZE 5000

#define YSIZE SIZE
#define XSIZE SIZE

int my_rank;

static inline double initial_value(int y, int x) {
    return 10 * y + x;
}

double **empty_matrix(int len_x, int len_y) {
    double **res = (double **) calloc(len_y, sizeof(double *));
    for (int i = 0; i < len_y; ++i) {
        res[i] = (double *) calloc(len_x, sizeof(double));
    }
    return res;
}   

void free_matrix(double **matrix, int len) {
    for (int t = 0; t < len; ++t) {
        free(matrix[t]);
    }
    free(matrix);
}

void initMatrix(double** matrix) {
    for (int i=0; i < YSIZE; i++){
        for (int j=0; j < XSIZE; j++){
            matrix[j][i] = 10*i +j;
        }
    }
}

void foldMatrixByVector(double **matrix, const double *vector, const int len) {
    int x0 = (int) vector[0];
    DB_PRINT("x0 = %i, len = %i\n", x0, len);
    for (int y = 0, i = 1; i < len; ++i, ++y) {
        matrix[x0][y] = vector[i];
        DB_PRINT("matrix[%i][%i] = vector[%i] = %lf\n", y, x0, i, matrix[x0][y]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1 && argc != 2) {
        printf("Wrong number of args! Please provide 2 arguments\n");
        exit(-1);
    }
    char* file;
    if (argc == 2) {
        file = argv[3];
    } else {
        file = "result_parallel.csv";
    }
    MPI_Init(&argc, &argv);
    double** matrix = NULL;
    int world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size == 1) {
        printf("world_size == 1");
        abort();
    }

    if(my_rank == 0) {
        matrix = empty_matrix(XSIZE, YSIZE);
        initMatrix(matrix);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double time_1 = MPI_Wtime();
    if (my_rank == 0) {
        double buf[YSIZE + 1];
        int messages = YSIZE;
        MPI_Status status;
        for (int i = 0; i < messages; ++i) {
            MPI_Recv(
                buf,
                YSIZE + 1,
                MPI_DOUBLE,
                MPI_ANY_SOURCE,
                MPI_ANY_SOURCE,
                MPI_COMM_WORLD,
                &status
            );
            int x0 = (int) buf[0];
            int message_len = 0;
            MPI_Get_count(&status, MPI_DOUBLE, &message_len);
            DB_PRINT("get message from %i, len = %i, x0 = %i\n",
                     status.MPI_SOURCE, message_len, x0);
            foldMatrixByVector(matrix, buf, message_len);}
        //DB_PRINT("after getting message, matrix[%i][%i] = %lf\n", 1, 17, matrix[1][17]);
        //DB_PRINT("after getting message, matrix[%i][%i] = %lf\n", 1, 16, matrix[1][16]);
    } else {
        // Not null rank
        int s = world_size - 1; // executing processes
        // Computing row roots
        double message[YSIZE + 1];
        int x0 = my_rank - 1;
        message[0] = (double) x0;
        double *arr = message + 1;
        for (int y = 0; y < YSIZE; ++y) {
            arr[y] = initial_value(y, x0);
        }
        MPI_Send(
            message,
            YSIZE + 1,
            MPI_DOUBLE,
            0, // the zero rank.
            0,
            MPI_COMM_WORLD
        );
        for (x0 = x0 + 8; x0 < XSIZE; x0 += 8) {
            message[0] = (double) x0;
            for (int y = YSIZE-1; y > 0; --y) {
                arr[y] = sin(5 * arr[y - 1]);
            }
            arr[0] = initial_value(0, x0);
            MPI_Send(
                message,
                YSIZE + 1,
                MPI_DOUBLE,
                0, // the zero rank.
                0,
                MPI_COMM_WORLD
            );
        }
    }
    double time_2 = MPI_Wtime();
    //DB_PRINT("%lf\n", time_2 - time_1);
    if (my_rank == 0) {
        printf("%lf", time_2 - time_1);
        FILE *ff;
        ff = fopen(file,"w");
        //DB_PRINT("matrix[%i][%i] = %lf\n", 1, 17, matrix[1][17]);
        //DB_PRINT("matrix[%i][%i] = %lf\n", 1, 16, matrix[1][16]);
        for(int y= 0; y < YSIZE; y++){
            for (int x= 0; x < XSIZE; x++){
                fprintf(ff, "%f ", matrix[x][y]);
                //printf("%f ",a[y][x]);
                if (x < XSIZE - 1) {
                    fprintf(ff,", ");
                    //printf(", ");
                }
            }
            fprintf(ff, "\n");
            //printf("\n");
        }
        fclose(ff);
        free_matrix(matrix, YSIZE);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

