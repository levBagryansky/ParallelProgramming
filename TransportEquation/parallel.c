#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define DB_PRINT(fmt, ...) printf("[MPI process %d, line %d]: " fmt, my_rank, __LINE__, ##__VA_ARGS__)
int my_rank;

#define X 1.0
#define T 1.0

int len_x;
int len_t;
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

void init_fragment(double **empty, int  start_x, int finish_x, int len_t) {
    if (start_x == 0) {
        for (int t = 0; t < len_t; ++t) {
            empty[t][0] = psi(tau * t);
        }
    }
    for (int x = 0; x < finish_x - start_x; ++x) {
        empty[0][x] = phi(h * (x + start_x));
    }
}

void print_matrix(double **matrix, int len_x, int len_t) {
    DB_PRINT("Printing matrix\n");
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

double inline static compute_point(double eq_eq, double eq_plus, double plus_eq, int x, int t) {
    return (2 * f(tau * (t), h * (x)) * tau - (plus_eq - eq_eq - eq_plus) - beta * alpha() * (eq_plus - eq_eq - plus_eq)) / (1 + beta * alpha());
}

double compute_point_matrix(double **matrix, int x, int t) {
    if (x && t) {
        double eq_eq = matrix[t-1][x-1];
        double eq_plus = matrix[t-1][x];
        double plus_eq = matrix[t][x-1];
        //double plus_plus = matrix[t][x];
        matrix [t][x] = compute_point(eq_eq, eq_plus, plus_eq, x, t);
    }
    return matrix[t][x];
}

/**
 * Calculates all points in section matrix. Receives points from
 *  the left section to compute own points.
 * @param section Fragment matrix which is calculated.
 * @param start_x Starting position in global matrix
 * @param finish_x Finishing position in global matrix.
 */
void compute_section(double **section, int start_x, int finish_x, int next_rank) {
    if (start_x == 0) {
        for (int t = 0; t < len_t; ++t) {
            for (int x = 0; x < finish_x - start_x; ++x) {
                // not already initialized
                if (x && t) {
                    section[t][x] = compute_point(
                            section[t - 1][x - 1],
                            section[t - 1][x],
                            section[t][x - 1],
                            start_x + x,
                            t
                    );
                }
            }
            MPI_Send(
                    section[t] + finish_x - start_x - 1,
                    1,
                    MPI_DOUBLE,
                    next_rank,
                    0,
                    MPI_COMM_WORLD
                    );
        }
    } else {
        double *prev_row = (double *) calloc(len_t, sizeof(double));
        for (int t = 0; t < len_t; ++t) {
            MPI_Status status;
            MPI_Recv(
                    prev_row + t,
                    1,
                    MPI_DOUBLE,
                    MPI_ANY_SOURCE,
                    MPI_ANY_TAG,
                    MPI_COMM_WORLD,
                    &status
                    );
            DB_PRINT("prev_row[%i] = %lf\n", t, prev_row[t]);
            for (int x = 0; x < finish_x - start_x; ++x) {
                if (t == 0){
                    ;
                } else {
                    double eq_eq, eq_plus, plus_eq;
                    eq_plus = section[t-1][x];
                    if (x == 0) {
                        eq_eq = prev_row[t-1];
                        plus_eq = prev_row[t];
                    } else {
                        eq_eq = section[t-1][x-1];
                        plus_eq = section[t][x-1];
                    }
                    section[t][x] = compute_point(eq_eq, eq_plus, plus_eq, start_x + x, t);
                }
            }
            MPI_Send(
                    section[t] + finish_x - start_x - 1,
                    1,
                    MPI_DOUBLE,
                    next_rank,
                    0,
                    MPI_COMM_WORLD
            );
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 4) {
        printf("Wrong number of args! Please provide 2 arguments\n");
        exit(-1);
    }
    char* file;
    if (argc == 4) {
        file = argv[3];
    } else {
        file = "solution.scv";
    }
    len_x = strtol(argv[1], NULL, 10);
    len_t = strtol(argv[2], NULL, 10);
    h = X / (len_x - 1);
    tau = T / (len_t - 1);
    beta = tau / h;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double **fragment;
    if (my_rank == 0) {
        fragment = empty_matrix(4, 10);
        init_fragment(fragment, 0, 4, 10);
        compute_section(fragment, 0, 4, 1);
        //print_matrix(fragment, 4, 10);
    } else if (my_rank == 1){
        fragment = empty_matrix(3, 10);
        init_fragment(fragment, 4, 7, 10);
        compute_section(fragment, 4, 7, 2);
        print_matrix(fragment, 3, 10);
    } else {
        fragment = empty_matrix(3, 10);
        init_fragment(fragment, 7, 10, 10);
        compute_section(fragment, 7, 10, -1);
        //print_matrix(fragment, 3, 10);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}