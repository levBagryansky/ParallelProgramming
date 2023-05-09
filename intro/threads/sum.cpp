#include <iostream>
#include <future>

double local_sum(int rank, int N, int threads) {
    int mod = rank % (threads);
    double local_sum = 0.0;
    for (int i = 0; 1 + mod + i * (threads) <= N; ++i) {
        double term = 1.0 / (1 + mod + i * (threads));
        local_sum += term;
    }
    return local_sum;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Please provide num_threads and num_series" << std::endl;
    }
    const int threads = (int) strtol(argv[1], nullptr, 10);
    const int N = (int) strtol(argv[2], nullptr, 10);

    std::future<double> all[threads];
    for (int i = 0; i < threads; ++i) {
        all[i] = std::async(&local_sum, i + 1, N, threads);
    }
    double sum = 0;
    for (int i = 0; i < threads; ++i) {
        sum += all[i].get();
    }
    std::cout << sum << std::endl;
    return 0;
}
