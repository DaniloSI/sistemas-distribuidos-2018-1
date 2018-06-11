#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
* Compilar: "gcc -o mc_serial montecarlo_serial.c -lm"
* Executar: "./mc_serial «npoints»"
* «npoints»: Número de pontos aleatórios.
*/

int inside_circle(double x, double y) {
    return sqrt((x*x) + (y*y)) <= 1.0;
}

double montecarlo(int npoints) {
    int i, circle_count = 0;

    srand(time(NULL));

    for (i = 0; i < npoints; i++) {
        double y = rand();
        double x = rand();

        while (x > 1.0) {
            x *= 0.1;
        }

        while (y > 1.0) {
            y *= 0.1;
        }

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
