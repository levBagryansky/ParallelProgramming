#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>

#define X 1.0
#define T 1.0

double h;
double tau;
// beta = (tau / h)
double beta;

static inline double phi(double x) {
    return cos(M_PI * x);
}

static inline double psi(double t) {
    return exp(-t);
}

static inline double alpha() {
    return 2.0;
}

static inline double f(double  x, double t) {
    return x + t;
}

double **empty_matrix(int len_x, int len_t) {
    double **res = (double **) calloc(len_t, sizeof(double *));
    for (int i = 0; i < len_t; ++i) {
        res[i] = (double *) calloc(len_x, sizeof(double));
    }
    return res;
}

void init_matrix(double **empty, int len_x, int len_t) {
    for (int i = 0; i < len_x; ++i) {
        empty[0][i] = phi(h * i);
    }
    for (int i = 0; i < len_t; ++i) {
        empty[i][0] = psi(tau * i);
    }
}

void print_matrix(double **matrix, int len_x, int len_t) {
//    printf("matrix:\n");
//    printf("matrix[0][0] = %lf\n", matrix[0][0]);
    for (int t = 0; t < len_t; ++t) {
        for (int x = 0; x < len_x; ++x) {
            if (x == len_x - 1) {
                printf("%lf ", matrix[t][x]);
            }
            else {
                printf("%lf, ", matrix[t][x]);
            }
        }
        printf("\n");
    }
}

double compute_point(double **matrix, int x, int t) {
    if (x && t) {
        double eq_eq = matrix[t-1][x-1];
        double eq_plus = matrix[t-1][x];
        double plus_eq = matrix[t][x-1];
        //double plus_plus = matrix[t][x];
        matrix [t][x] = (2 * f(tau * (t), h * (x)) * tau - (plus_eq - eq_eq - eq_plus) - beta * alpha() * (eq_plus - eq_eq - plus_eq)) / (1 + beta * alpha());
    }
    return matrix[t][x];
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Wrong number of args! Please provide 2 arguments\n");
        exit(-1);
    }
    int len_x = strtol(argv[1], NULL, 10);
    int len_t = strtol(argv[2], NULL, 10);
    h = X / (len_x - 1);
    tau = T / (len_t - 1);
    beta = tau / h;
    double **matrix = empty_matrix(len_x, len_t);
    init_matrix(matrix, len_x, len_t);
    //print_matrix(matrix, len_x, len_t);
    for (int t = 0; t < len_t; ++t) {
        for (int x = 0; x < len_x; ++x) {
            compute_point(matrix, x, t);
        }
    }
    print_matrix(matrix, len_x, len_t);
    return 0;
}
