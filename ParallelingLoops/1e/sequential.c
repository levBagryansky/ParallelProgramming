#include <math.h>
#include <bits/types/FILE.h>
#include <stdio.h>

#define YSIZE 10
#define XSIZE 10

int main(int argc, char **argv) {
    char *out = "result.csv";
    if (argc == 2) {
        out = argv[1];
    }
    double a[YSIZE][XSIZE];
    int y, x;
    FILE *ff;
    for (y=0; y < YSIZE; y++){
        for (x=0; x < XSIZE; x++){
            a[y][x] = 10 * y + x;
        }
    }

//начало измерения времени
    for (y=1; y < YSIZE; y++){
        for (x = 8; x < XSIZE; x++){
            a[y][x] = sin(5 * a[y - 1][x - 8]);
        }
    }
//окончание измерения времени

    ff = fopen(out,"w");
    for(y= 0; y < YSIZE; y++){
        for (x= 0; x < XSIZE; x++){
            fprintf(ff,"%f ",a[y][x]);
            printf("%f ",a[y][x]);
            if (x < XSIZE - 1) {
                fprintf(ff,", ");
                printf(", ");
            }
        }
        fprintf(ff, "\n");
        printf("\n");
    }
    fclose(ff);
}
