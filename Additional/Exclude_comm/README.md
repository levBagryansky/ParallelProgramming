# Creating a custom MPI communicator
### The program creates a custom communicator and compute the sum of N terms of harmonic series.
## Running
### From project directory run
```
$ cmake . -B cmake-build-debug && cd cmake-build-debug
$ make
$ mpirun -np <number of processes> exclude_communicator <N>
```
### The result will be as follows:
```
Hello from process 0 out of 7
new_comm == MPI_COMM_NULL, exit
Hello from process 1 out of 7
Hello from process 2 out of 7
Hello from process 3 out of 7
Hello from process 4 out of 7
Hello from process 5 out of 7
Hello from process 6 out of 7
result = 2.928968
```