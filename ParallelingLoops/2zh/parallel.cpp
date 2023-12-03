#include <cstdio>
#include <string>
#include <omp.h>
#include <cmath>

constexpr int SIZE = 5000;
constexpr int YSIZE = SIZE;
constexpr int XSIZE = SIZE;

double **empty_matrix(int len_y, int len_x);
void free_matrix(double **matrix, int len_y);
static inline void fill_vector(int start_y, int start_x, double** matrix) {
    for (int y = start_y, x = start_x; x >= 3 && y < YSIZE - 2; x -= 3, y += 2) {
        matrix[y][x] = sin(0.1 * matrix[y+2][x-3]);
    }
}

int main (int argc, char **argv) {
    std::string out = "result_parallel.csv";
    if (argc == 3) {
        out = argv[2];
    }
    double** matrix = empty_matrix(YSIZE, XSIZE);
    FILE *ff;
    for (int y=0; y < YSIZE; y++){
        for (int x=0; x < XSIZE; x++){
            matrix[y][x] = 10 * y + x;
        }
    }

//начало измерения времени
    double time_1 = omp_get_wtime();
    omp_set_num_threads(strtol(argv[1], nullptr, 10));
#pragma omp parallel
    {
        for (int x = XSIZE - 3; x < XSIZE; ++x) {
#pragma omp for nowait schedule(dynamic, 100)
            for (int y = 0; y < YSIZE; ++y) {
                fill_vector(y, x, matrix);
            }
        }
        for (int y = 0; y < 2; ++y) {
#pragma omp for nowait schedule(dynamic, 100)
            for (int x = 0; x < XSIZE - 3; ++x) {
                fill_vector(y, x, matrix);
            }
        }
    }
    double time_2 = omp_get_wtime();
//окончание измерения времени
    printf("%lf", time_2 - time_1);
    fflush(stdout);
    ff = fopen(out.c_str(),"w");
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
    free_matrix(matrix, YSIZE);
    fclose(ff);

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
