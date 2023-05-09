#include <iostream>
#include <cmath>
#include <mutex>

static inline double fun(double x) {
    return sin(1.0 / x);
}

static inline double max_first_dir(double x) {
    return fabs(2 / (x * x * x)) + fabs(1.0 / (x * x * x * x));
}

const double finish_x = 1.0;
const int portion = 1000;
std::mutex mutex_x;
std::mutex mutex_sum;

int main(int argc, char** argv) {
    if (argc != 3){
        std::cout << "Please provide <n_threads> and <error> only" << std::endl;
        exit(-1);
    }
    double curr_x = 0.001;
    double sum = 0.0;
    int threads = (int) strtol(argv[1], nullptr, 10);
    const double specific_error = strtod(argv[2], nullptr) / finish_x;

    std::cout << "Hello, World!" << std::endl;
    auto f = [&curr_x, &sum, specific_error]() {
        while (curr_x <= finish_x) {
            mutex_x.lock();
            double h = sqrt(12 * specific_error / max_first_dir(curr_x));
            double loc_x = curr_x;
            curr_x += (portion * h);
            mutex_x.unlock();
            if (loc_x >= finish_x) {
                break;
            }
            if (loc_x + h * portion >= finish_x) {
                double loc_sum = 0.0;

            } else {
                double loc_sum = (fun(curr_x) + fun(curr_x + portion * h)) / 2;
                for (int i = 1; i < portion; ++i) {
                    loc_x += h;
                    loc_sum += fun(loc_x);
                }
                loc_sum *= h;
                mutex_sum.lock();
                sum += loc_sum;
                mutex_sum.unlock();
            }
        }
    }

    return 0;
}
