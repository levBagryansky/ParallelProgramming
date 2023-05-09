#include <iostream>
#include <thread>
#include <csignal>
#include <mutex>

std::mutex mutex;
int cell = 0;

int main(int argc, char **argv) {
    int threads = 10;
    if (argc == 2) {
        threads = static_cast<int> (strtol(argv[1], NULL, 10));
    }
    auto f = []()
    {
        mutex.lock();
        cell += 1;
        mutex.unlock();
    };
    std::thread all[threads];
    for (int i = 0; i < threads; ++i) {
        all[i] = std::thread(f);
    }
    for (int i = 0; i < threads; ++i) {
        all[i].join();
    }
    std::cout << cell << std::endl;
    return cell;
}
