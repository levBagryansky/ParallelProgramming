#include <cstdio>
#include <cmath>
#include <omp.h>
#include <string>

constexpr int SIZE = 5000;
constexpr int YSIZE = SIZE;
constexpr int XSIZE = SIZE;

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
    std::string out = "result.csv";
    if (argc == 2) {
        out = argv[1];
    }
    double** a = empty_matrix(YSIZE, XSIZE);
    double** b = empty_matrix(YSIZE, XSIZE);
    int y, x;
    FILE *ff;
    for (y=0; y < YSIZE; y++){
        for (x=0; x < XSIZE; x++){
            a[y][x] = 10 * y + x;
        }
    }

//начало измерения времени
    double time_1 = omp_get_wtime();
    for (int i=0; i < YSIZE; i++){
        for (int j = 0; j < XSIZE; j++){
            a[i][j] = sin(0.1*a[i][j]);
        }
    }
    for (int i=1; i<YSIZE; i++){
        for (int j = 0; j < XSIZE-2; j++){
            b[i][j] = a[i-1][j+2]*1.5;
        }
    }
    double time_2 = omp_get_wtime();
//окончание измерения времени

    printf("%lf", time_2 - time_1);
    ff = fopen(out.c_str(),"w");
    for(y= 0; y < YSIZE; y++){
        for (x= 0; x < XSIZE; x++){
            fprintf(ff,"%f ",b[y][x]);
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
    free_matrix(b, YSIZE);
    fclose(ff);
}

