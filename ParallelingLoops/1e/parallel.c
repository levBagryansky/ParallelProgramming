#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/**
 * Macros. Appends rank and line before the message.
 */
#define DB_PRINT(fmt, ...) ; printf("[MPI process %d, line %d]: " fmt, my_rank, __LINE__, ##__VA_ARGS__)

#define SIZE 5000

#define YSIZE SIZE
#define XSIZE SIZE

int my_rank;

static inline double initial_value(int y, int x) {
    return 10 * y + x;
}

double **empty_matrix(int len_y, int len_x) {
    double **res = (double **) calloc(len_y, sizeof(double *));
    for (int i = 0; i < len_y; ++i) {
        res[i] = (double *) calloc(len_x, sizeof(double));
    }
    return res;
}

void free_matrix(double **matrix, int len_y) {
    for (int t = 0; t < len_y; ++t) {
        free(matrix[t]);
    }
    free(matrix);
}

void initMatrix(double** matrix) {

    for (int i=0; i < YSIZE; i++){
        for (int j=0; j < XSIZE; j++){
            matrix[i][j] = 10*i +j;
        }
    }
}

void foldMatrixByVector(double **matrix, const double *vector, const int len) {
    int y0 = (int) vector[0];
    int x0 = (int) vector[1];
    //DB_PRINT("y0 = %i, x0 = %i, len = %i\n", y0, x0, len);
    for (int x = x0, y = y0, i = 2; i < len; ++i, ++y, x += 8) {
        matrix[y][x] = vector[i];
        //DB_PRINT("matrix[%i][%i] = vector[%i] = %lf\n", y, x, i, matrix[y][x]);
        if(x >= XSIZE) {
            //DB_PRINT("x >= XSIZE, x = %i\n", x);
        }
        if(y >= YSIZE) {
            //DB_PRINT("y >= YSIZE\n");
        }
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
        matrix = empty_matrix(YSIZE, XSIZE);
        initMatrix(matrix);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double time_1 = MPI_Wtime();
    if (my_rank == 0) {
        double buf[XSIZE / 7];
        int roots =  XSIZE - 8 + YSIZE * 8;
        MPI_Status status;
        for (int i = 0; i < roots; ++i) {
            MPI_Recv(
                buf,
                XSIZE,
                MPI_DOUBLE,
                MPI_ANY_SOURCE,
                MPI_ANY_SOURCE,
                MPI_COMM_WORLD,
                &status
            );
            int y0 = (int) buf[0];
            int x0 = (int) buf[1];
            int message_len = 0;
            MPI_Get_count(&status, MPI_DOUBLE, &message_len);
//            DB_PRINT("get message from %i, len = %i, y0 = %i, x0 = %i\n",
//               status.MPI_SOURCE, message_len, y0, x0);
            foldMatrixByVector(matrix, buf, message_len);}
        //DB_PRINT("after getting message, matrix[%i][%i] = %lf\n", 1, 17, matrix[1][17]);
        //DB_PRINT("after getting message, matrix[%i][%i] = %lf\n", 1, 16, matrix[1][16]);
    } else {
        // Not null rank
        int s = world_size - 1; // executing processes
        // Computing row roots
        double message[XSIZE / 8 + 10];
        for (int curr_y0 = my_rank - 1; curr_y0 < YSIZE; curr_y0 += s) {
            for (int curr_x0 = 0; curr_x0 < 8; ++curr_x0) {
                message[0] = (double) curr_y0;
                message[1] = (double) curr_x0;
                //DB_PRINT("curr_y0 = %i, curr_x0 = %i\n", curr_y0, curr_x0);
                double *arr = message + 2;
                int y = curr_y0;
                int x = curr_x0;
                arr[0] = initial_value(curr_y0, curr_x0);
                //DB_PRINT("arr[%i] = %lf\n", 0, arr[0]);
                int i = 1;
                y += 1;
                x += 8;
                while (y < YSIZE && x < XSIZE) {
                    arr[i] = sin(5 * arr[i - 1]);
                    //DB_PRINT("arr[%i] = %lf, y = %i, x = %i\n", i, arr[i], y, x);
                    i++;
                    y += 1;
                    x += 8;
                }
                //DB_PRINT("i after first while = %i\n", i);
                MPI_Send(
                    message,
                    i + 2,
                    MPI_DOUBLE,
                    0, // the zero rank.
                    0,
                    MPI_COMM_WORLD
                );
            }
        }

        for (int curr_x0 = (my_rank-1) % 8 + 8; curr_x0 < XSIZE; curr_x0 += s) {
            if ((my_rank-1) % 8 != curr_x0) {
                //DB_PRINT("(%i-1) mod 8 + 8 != %i\n", my_rank, curr_x0);
            }
            message[0] = 0.0;
            message[1] = (double) curr_x0;
            int curr_y0 = 0;
            int x = curr_x0;
            int y = curr_y0;
            //DB_PRINT("curr_y0 = %i, curr_x0 = %i\n", curr_y0, curr_x0);
            double *arr = message + 2;
            arr[0] = initial_value(curr_y0, x);
            y += 1;
            x += 8;
            int i = 1;
            while (y < YSIZE && x < XSIZE) {
                arr[i] = sin(5 * arr[i - 1]);
                //DB_PRINT("arr[%i] = %lf, y = %i, x = %i\n", i, arr[i], y, x);
                i++;
                y += 1;
                x += 8;
            }
            //DB_PRINT("i after second while = %i\n", i);
            MPI_Send(
                message,
                i + 2,
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
                fprintf(ff, "%f ", matrix[y][x]);
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

