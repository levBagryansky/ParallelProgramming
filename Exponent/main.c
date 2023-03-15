#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int size = 10000000;

int SendFloat(mpf_t *mpf, int rank);

void fact(int n);

void term(int n, mpf_t p) {
    int i;

    mpf_init_set_ui(p,1); /* p = 1 */
    for (i=1; i <= n ; ++i){
        mpf_div_ui(p,p,i); /* p = p / i */
    }

//    printf ("1 / (%d!)  =  ", n);
//    fflush(stdout);
//    mpf_out_str(stdout,10, 10,p);
//    printf("\n");
}

int main(int argc, char * argv[]){
    int n;
    if (argc <= 1){
        printf ("Usage: %s <number> \n", argv[0]);
        return 2;
    }
    n = atoi(argv[1]);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {

    }

    mpf_set_default_prec(10000);
    assert( n >= 0);
    mpf_t mpf;
    term(n, mpf);
}

void fact(int n){
    int i;
    mpz_t p;

    mpz_init_set_ui(p,1); /* p = 1 */
    for (i=1; i <= n ; ++i){
        mpz_mul_ui(p,p,i); /* p = p * i */
    }
    printf ("%d!  =  ", n);
    mpz_out_str(stdout,10,p);
    printf("\n");

    char* str = mpz_get_str(NULL, 10, p);
    for (int i = 0; i < size; ++i) {
        printf("%c", str[i]);
        if (str[i - 1] != 0 && str[i] == 0) {
            printf("\ni = %i\n", i);
            break;
        }
        fflush (stdout);
    }
}
