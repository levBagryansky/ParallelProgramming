#include <omp.h>
#include <iostream>

int main () {
    omp_set_num_threads(5);
    int cell = 0;

#pragma omp parallel shared(cell, std::cout) default(none)
    {
        int id = omp_get_thread_num();
        while (cell != id) {}
#pragma omp critical
        {
            std::cout << "[Thread " << id << "]: cell before inc = " << cell << std::endl;
            int saved;
            saved = ++cell;
            std::cout << "[Thread " << id << "]: cell after  inc = " << saved << std::endl;
        }
    }
    return 0;
}
/*
 * OUTPUT:
 * [Thread 0]: cell before inc = 0
 * [Thread 0]: cell after  inc = 1
 * [Thread 1]: cell before inc = 1
 * [Thread 1]: cell after  inc = 2
 * [Thread 2]: cell before inc = 2
 * [Thread 2]: cell after  inc = 3
 * [Thread 3]: cell before inc = 3
 * [Thread 3]: cell after  inc = 4
 * [Thread 4]: cell before inc = 4
 * [Thread 4]: cell after  inc = 5
 */
