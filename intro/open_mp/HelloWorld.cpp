#include <omp.h>
#include <cstdio>

int main () {
    omp_set_num_threads(10);

#pragma omp parallel default(none)
    printf("thread %i/%i: Hello World\n", omp_get_thread_num(), omp_get_max_threads());
    return 0;
}
// *************************
// Output:
// thread 0/10: Hello World
// thread 2/10: Hello World
// thread 6/10: Hello World
// thread 9/10: Hello World
// thread 4/10: Hello World
// thread 7/10: Hello World
// thread 8/10: Hello World
// thread 1/10: Hello World
// thread 5/10: Hello World
// thread 3/10: Hello World