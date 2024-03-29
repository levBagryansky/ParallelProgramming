#include <iostream>
#include <cmath>
#include <chrono>
#include <mutex>
#include <thread>

static inline long double fun(long double x) {
    return sinl(1.0L / x);
}

static inline long double max_first_dir(long double x) {
    return fabsl(2 / (x * x * x)) + fabsl((long double) 1.0 / (x * x * x * x));
}

const long double finish_x = 1.0;
int portion;
std::mutex mutex_x;
std::mutex mutex_sum;

int main(int argc, char** argv) {
    if (argc != 3){
        std::cout << "Please provide <n_threads> and <error> only" << std::endl;
        exit(-1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    long double curr_x = 0.001;
    long double sum = 0.0;
    int threads = (int) strtol(argv[1], nullptr, 10);
    const long double specific_error = strtod(argv[2], nullptr) / finish_x;
    //printf("spec_error = %.100Lf\n", specific_error);
    portion  = static_cast<int>(1.0 / (threads * sqrtl(specific_error)));
    if (portion < 50) {
        portion = 50;
    }
    //printf("portion = %i\n", portion);
    auto f = [&curr_x, &sum, specific_error]() {
        int statistic = 0;
        while (curr_x <= finish_x) {
            mutex_x.lock();
            long double h = sqrtl(
                specific_error * ((long double) 12.0) / max_first_dir(curr_x)
            );
            long double loc_x = curr_x;
            curr_x += (portion * h);
            mutex_x.unlock();
            if (loc_x >= finish_x) {
                break;
            }
            long double loc_sum = 0.0;
            if (loc_x + h * portion >= finish_x) {
                const int iters = static_cast<int>((finish_x - loc_x) / h);
                for (int i = 0; i < iters; ++i) {
                    loc_sum += 0.5 * (fun(loc_x) + fun(loc_x + h));
                    loc_x += h;
                }
                loc_sum *= h;
                loc_sum += 0.5 * (finish_x - loc_x) * (fun(loc_x) + fun(finish_x));
            } else {
                loc_sum = (fun(loc_x) + fun(loc_x + portion * h)) / 2;
                for (int i = 1; i < portion; ++i) {
                    loc_x += h;
                    loc_sum += fun(loc_x);
                }
                loc_sum *= h;
            }
            mutex_sum.lock();
            sum += loc_sum;
            mutex_sum.unlock();
            statistic++;
        }
        //printf("this thread made %i portions\n", statistic);
    };
    std::thread all[threads];
    for (int i = 0; i < threads; ++i) {
        all[i] = std::thread(f);
    }
    for (int i = 0; i < threads; ++i) {
        all[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout <<
              std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //printf("\ntime = %0.20Lf\n", sum);
    return 0;
}
