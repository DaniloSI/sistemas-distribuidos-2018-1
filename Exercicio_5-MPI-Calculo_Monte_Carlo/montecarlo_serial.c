#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

/**
* Compilar: "gcc -o mc_serial montecarlo_serial.c -lm"
* Executar: "./mc_serial «npoints»"
* «npoints»: Número de pontos aleatórios.
*/

int inside_circle(double x, double y) {
    return sqrt((x*x) + (y*y)) <= 1.0;
}

double get_rand_num() {
    struct timeval te;
    gettimeofday(&te, NULL);
    srand((unsigned)time(NULL) + te.tv_usec);
    return (double)(rand() % 10000001) / (double)10000001;
}

double montecarlo(int npoints) {
    int i, circle_count = 0;

    for (i = 0; i < npoints; i++) {
        double y = get_rand_num();
        double x = get_rand_num();

        if (inside_circle(x, y)) {
            circle_count++;
        }
    }

    return (4.0 * circle_count) / ((double) npoints);
}

int main(int argc, char const *argv[]) {
    int npoints;
    double pi;
    clock_t start, stop;

    start = clock();

    npoints = atoi(argv[1]);

    pi = montecarlo(npoints);

    printf("Valor de PI: %lf\n", pi);

    stop = clock();

    printf("Tempo gasto na execução: %.3lf segundos.\n", ((double)(stop - start) / CLOCKS_PER_SEC));

    return 0;
}
