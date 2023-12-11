#include <cstdio>
#include <string>
#include <omp.h>
#include <cmath>
#include <cstring>

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
    const int threads = strtol(argv[1], nullptr, 10);
    if (threads <= 1) {
        for (int y=0; y < YSIZE - 2; y++){
            for (int x = 3; x < XSIZE; x++){
                matrix[y][x] = sin(0.1 * matrix[y+2][x-3]);
            }
        }
        goto Finish;
    }
    omp_set_num_threads(threads);
#pragma omp parallel
    {
        int curr_thr = omp_get_thread_num();
        int group = curr_thr % 2;
        int id_in_group = curr_thr / 2;
        int threads_in_group, lines_of_group;
        if (group) {
            threads_in_group = threads / 2;
            lines_of_group = YSIZE / 2;
        } else {
            threads_in_group = (threads + 1) / 2;
            lines_of_group = (YSIZE + 1) / 2;
        }
        int lines = lines_of_group / threads_in_group;
        int start = lines * id_in_group * 2;
        if (group) {
            start++;
        }
        double saved[XSIZE];
        if (id_in_group == threads_in_group - 1) {
            lines += (lines_of_group % threads_in_group);
            // printf("[thread %i]: start with %i\n", curr_thr, start);
        } else {
            std::memcpy(saved, reinterpret_cast<const void *>(matrix[start + lines * 2]), XSIZE * sizeof(double));
            // printf("[thread %i]: start with %i, save line %i\n", curr_thr, start, start + lines * 2);
        }
#pragma omp barrier
        int y;
        for (y = start; y < start + lines * 2 - 2; y += 2) {
            for (int x = 3; x < XSIZE; ++x) {
                matrix[y][x] = sin(0.1 * matrix[y+2][x-3]);
            }
        }
        if (id_in_group != threads_in_group - 1) {
            for (int x = 3; x < XSIZE; ++x) {
                matrix[y][x] = sin(0.1 * saved[x-3]);
            }
        }
        //printf("thr %i => %i\n", curr_thr, curr_thr / 2);
    }
    Finish:
    double time_2 = omp_get_wtime();
//окончание измерения времени
    printf("%lf", time_2 - time_1);
    //fflush(stdout);
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
