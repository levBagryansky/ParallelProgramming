# introductory thread tasks 
### To build the project:
```shell
cmake . -B build/ 
cd build && make
```
## sum 
### Counts the sum of the first N terms of the harmonic series. Run:
```shell
./Sum <num_of_threads> <num_of_terms>
```
## Hello
### Every thread prints its number and total threads count
```shell
./Hello <num_of_threads>
```
## Sequential Access.
### Every thread accesses to the `cell` in order and increments it.
```shell
./SequentialAccess <num_of_threads>
```