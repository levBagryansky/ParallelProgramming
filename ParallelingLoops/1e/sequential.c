#include <math.h>
#include <bits/types/FILE.h>
#include <stdio.h>

#define ISIZE 10
#define JSIZE 10

int main(int argc, char **argv) {
    char *out = "result.csv";
    if (argc == 2) {
        out = argv[1];
    }
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }

//начало измерения времени
    for (i=1; i<ISIZE; i++){
        for (j = 8; j < JSIZE; j++){
            a[i][j] = sin(5*a[i-1][j-8]);
        }
    }
//окончание измерения времени

    ff = fopen(out,"w");
    for(i= 0; i < ISIZE; i++){
        for (j= 0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
            printf("%f ",a[i][j]);
            if (j < JSIZE - 1) {
                fprintf(ff,", ");
                printf(", ");
            }
        }
        fprintf(ff, "\n");
        printf("\n");
    }
    fclose(ff);
}
