#include <omp.h>
#include <iostream>

double sumPart(int id, int threads, int N);

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Wrong number of args" << std::endl;
        abort();
    }
    int N = strtol(argv[1], nullptr, 10);

    omp_set_num_threads(10);
    int threads = omp_get_max_threads();
    double loc_sum;
#pragma omp parallel reduction(+ : loc_sum) shared(threads, N) default(none)
    loc_sum = sumPart(omp_get_thread_num(), threads, N);
    std::cout << loc_sum << std::endl;
    return 0;
}


double sumPart(int id, int threads, int N) {
    if (id == 0) {
        return 0.0;
    }
    // Compute a local sum several term
    int mod = id % (threads - 1);
    double local_sum = 0.0;
    for (int i = 0; 1 + mod + i * (threads - 1) <= N; ++i) {
        double term = 1.0 / (1 + mod + i * (threads - 1));
        local_sum += term;
    }
    return local_sum;
}
