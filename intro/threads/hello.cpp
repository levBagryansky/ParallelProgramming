#include <iostream>
#include <thread>
#include <csignal>

int main(int argc, char **argv) {
    int threads = 10;
    if (argc == 2) {
        threads = static_cast<int> (strtol(argv[1], NULL, 10));
    }
    auto f = [&threads](int x)
    {
        sleep(0.3);
        printf("Hello world, i = %i, in total %i\n", x, threads);
    };
    std::thread* all[threads];
    for (int i = 0; i < threads; ++i) {
        all[i] = new std::thread(f, i);
    }
    for (int i = 0; i < threads; ++i) {
        all[i]->join();
        delete all[i];
    }
    return 0;
}
