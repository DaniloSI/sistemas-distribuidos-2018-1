#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

/**
* Compilar: "mpicc montecarlo_mpi.c -o montecarlo_mpi -lm -Wall"
* Executar: "mpirun -np «numeroprocessos» ./montecarlo_mpi «npoints»"
* «npoints»: Número de pontos aleatórios.
*/

#define ROOT 0

int inside_circle(double x, double y) {
    return sqrt((x*x) + (y*y)) <= 1.0;
}

double get_rand_num() {
    struct timeval te;
    gettimeofday(&te, NULL);
    srand((unsigned)time(NULL) + te.tv_usec);
    return (double)(rand() % 10000001) / (double)10000001;
}

double montecarlo_inside_circle(int rank, int npoints) {
    int i, circle_count = 0;

    for (i = 0; i < npoints; i++) {
        double y = get_rand_num();
        double x = get_rand_num();

        if (inside_circle(x, y)) {
            circle_count++;
        }
    }

    return circle_count;
}

double calculo_pi(int circle_count, int npoints) {
    return (4.0 * ((double) circle_count)) / ((double) npoints);
}

int main(int argc, char **argv) {
    int size, rank, npoints, insidecircle;
    clock_t start, stop;

    start = clock();

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    npoints = atoi(argv[1]);

    insidecircle = montecarlo_inside_circle(rank, npoints / size);

    if (rank == ROOT) {
        int total_insidecircle;
        double pi;

        MPI_Reduce(&insidecircle, &total_insidecircle, 1, MPI_INT,
            MPI_SUM, ROOT, MPI_COMM_WORLD);

        pi = calculo_pi(total_insidecircle, npoints);

        printf("Valor do PI: %lf\n", pi);

    } else {
        MPI_Reduce(&insidecircle, NULL, 1, MPI_INT, MPI_SUM,
            ROOT, MPI_COMM_WORLD);
    }

    stop = clock();

    if (rank == ROOT) {
        printf("Tempo gasto na execução: %.3lf segundos.\n", ((double)(stop - start) / CLOCKS_PER_SEC));
    }

    MPI_Finalize();

    return 0;
}
