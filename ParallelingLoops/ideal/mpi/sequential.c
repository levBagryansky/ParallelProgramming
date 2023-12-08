#include <math.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 5000

#define YSIZE SIZE
#define XSIZE SIZE

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

int main(int argc, char **argv) {
    char *out = "result.csv";
    if (argc == 2) {
        out = argv[1];
    }
    double** a = empty_matrix(YSIZE, XSIZE);
    int y, x;
    FILE *ff;
    for (y=0; y < YSIZE; y++){
        for (x=0; x < XSIZE; x++){
            a[y][x] = 10 * y + x;
        }
    }

//начало измерения времени
    double time_1 = MPI_Wtime();
    for (y=0; y < YSIZE; y++){
        for (x = 0; x < XSIZE; x++){
            a[y][x] = sin(2*a[y][x]);
        }
    }
    double time_2 = MPI_Wtime();
    printf("%lf", time_2 - time_1);
//окончание измерения времени

    ff = fopen(out,"w");
    for(y= 0; y < YSIZE; y++){
        for (x= 0; x < XSIZE; x++){
            fprintf(ff,"%f ",a[y][x]);
            //printf("%f ",a[y][x]);
            if (x < XSIZE - 1) {
                fprintf(ff,", ");
                //printf(", ");
            }
        }
        fprintf(ff, "\n");
        //printf("\n");
    }
    free_matrix(a, YSIZE);
    fclose(ff);

}
