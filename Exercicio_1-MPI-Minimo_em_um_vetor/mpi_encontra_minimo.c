#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

/**
 * Compilar: "mpicc mpi_encontra_minimo.c -o mpi_encontra_minimo -Wall"
 * Executar: "mpirun -np «numeroprocessos» ./mpi_encontra_minimo «numeroposicoesvetor» «epsilon»"
 */

#define ROOT 0

void preenche_vetor_aleatorio(double*, int);
void multiplica_valores(double*, int, double);
double minimo_local(double*, int);

int main(int argc, char *argv[]) {
    int size, rank, p, scount, rcount;
    double *sbuf, *rbuf, epsilon;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    p = atoi(argv[1]);
    epsilon = atof(argv[2]);
    scount = rcount = p/size;

    if (rank == ROOT) {
        int niteracoes = 0;
        double svetor[p], rvetor[rcount], global_min;

        preenche_vetor_aleatorio(svetor, p);
        sbuf = svetor;
        rbuf = rvetor;
        MPI_Scatter(sbuf, scount, MPI_DOUBLE, rbuf, rcount, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
        do {
            double local_min;

            multiplica_valores(rvetor, rcount, 0.9);
            local_min = minimo_local(rvetor, rcount);
            MPI_Allreduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
            niteracoes++;
        } while(global_min > epsilon);

        printf("Numero de iteracoes: %d\nMenor valor: %lf\n", niteracoes, global_min);
    } else {
        double rvetor[rcount], global_min;

        rbuf = rvetor;
        MPI_Scatter(sbuf, scount, MPI_DOUBLE, rbuf, rcount, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
        do {
            double local_min;

            multiplica_valores(rvetor, rcount, 0.9);
            local_min = minimo_local(rvetor, rcount);
            MPI_Allreduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        } while(global_min > epsilon);
    }

    MPI_Finalize();

    return 0;
}

void preenche_vetor_aleatorio(double* vetor, int p) {
    int i;
    srand((unsigned)time(NULL));

    for (i = 0; i < p; i++) {
        vetor[i] = rand() % 101;
    }
}

void multiplica_valores(double* vetor, int n, double valor) {
    int i;
    for (i = 0; i < n; i++) {
        vetor[i] *= valor;
    }
}

double minimo_local(double* vetor, int n) {
    double local_min = vetor[0];
    int i;

    for (i = 1; i < n; i++) {
        if (local_min > vetor[i])
            local_min = vetor[i];
    }

    return local_min;
}
