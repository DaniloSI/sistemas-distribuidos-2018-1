#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/**
 * Compilar: "mpicc hello_mpi.c -o hello_mpi -Wall -lm"
 * Executar: "mpirun -np «numeroprocessos» ./hello_mpi «intervalo_a» «intervalo_b»"
 */

#define ROOT 0

double f(double);

double area_trapezio(double, double, double);

void erro(char*);

int main(int argc, char *argv[])
{
    int size, rank;
    double a, b, dx;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        erro("Faltou o intervalo para calcular a integral.");
    }

    if (size < 2) {
        erro("Eh preciso haver mais de 1 processo.");
    }

    a = atof(argv[1]);
    b = atof(argv[2]);

    if (b == a) {
        erro("Os intervalos devem ser valores diferentes.");
    } else if (abs(b) > 1 || abs(a) > 1) {
        erro("Cada intervalo da integral deve ser um valor no intervalo [-1, 1].");
    } else if (b < a) {
        double aux = b;
        b = a;
        a = aux;
    }

    dx = (b - a)/(size - 1);

    if (rank == ROOT) {
        double integral_r = 0.0;
        int i;

        for (i = 1; i < size; i++) {
            MPI_Status status;
            double area_tr;

            MPI_Recv(&area_tr, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            integral_r += area_tr;
        }

        printf("Resultado da integral: %lf\n", integral_r);
    } else {
        double x = a + (dx * (rank - 1));
        double f1 = f(x);
        double f2 = f(x + dx);
        double area_tr = area_trapezio(dx, f1, f2);

        MPI_Send(&area_tr, 1, MPI_DOUBLE, ROOT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

double f(double x) {
    return 4 * sqrt(1 - (x*x));
}

double area_trapezio(double dx, double y1, double y2) {
    return (dx * (y1 + y2))/2;
}

void erro(char* msg) {
    printf("[ERRO] %s\n", msg);
    MPI_Finalize();
    exit(0);
}
