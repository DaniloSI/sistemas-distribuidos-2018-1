#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>

/**
 * Compilar: "mpicc mpi_ordenacao.c -o mpi_ordenacao -Wall"
 * Executar: "mpirun -np «numeroprocessos» ./mpi_ordenacao «pposicoes»"
 */

 #define ROOT 0

int rand_num(int begin, int end) {
    struct timeval te;
    gettimeofday(&te, NULL);
    srand((unsigned)time(NULL) + te.tv_usec);
    return (rand() % (end - begin + 1)) + begin;
}

void print_vetor(int* vetor, int size) {
    int i;

    for (i = 0; i < size; i++) {
        printf("Index: %d | Value: %d\n", i, vetor[i]);
    }
}

void ordena_vetor(int* vetor, int size) {
    int i, j, aux;

    printf("Entrou no ordena_vetor\n");

    for (i = 0; i < size; i++) {
        for (j = i; j < size; j++) {
            if (vetor[j] < vetor[i]) {
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }

    printf("Saiu do ordena_vetor\n");
}

 int main(int argc, char **argv) {
     int size, rank, p;

     MPI_Init(&argc, &argv);

     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

     p = atoi(argv[1]);

     if (rank == ROOT) {
         int recvcount = p / size;
         int vrecv[recvcount], v[p];
         int i;

         for (i = 0; i < p; i++) {
             v[i] = rand_num(0, 100);
         }

         //print_vetor(v, p);

         MPI_Scatter(v, size, MPI_INT, vrecv, recvcount, MPI_INT, ROOT, MPI_COMM_WORLD);

         ordena_vetor(vrecv, recvcount);

         MPI_Gather(vrecv, recvcount, MPI_INT, v, p, MPI_INT, ROOT, MPI_COMM_WORLD);

         //print_vetor(v, p);
         print_vetor(v, p);
     } else {
         int recvcount = p / size;
         int vrecv[recvcount], v[p];

         MPI_Scatter(v, p, MPI_INT, vrecv, recvcount, MPI_INT, ROOT, MPI_COMM_WORLD);

         ordena_vetor(vrecv, recvcount);

         MPI_Gather(vrecv, recvcount, MPI_INT, v, p, MPI_INT, ROOT, MPI_COMM_WORLD);
     }

     MPI_Finalize();

     return 0;
 }


/* ETAPAS:
 * - [ ] Todos os processos ordenar seus vetores localmente, para facilitar a
 *       ordenacao global.
 * - [ ] O processo Root imprimir o vetor ordenado.
 * - [ ] Todos os processos devem finalizar.
 * - [x] O Root criar um vetor V de P posicoes preenchido com numeros aleatorios
 *       inteiros entre 0 e 100 e imprimir o vetor gerado.
 * - [x] Distribuir o vetor em N processos, incluindo si proprio.
 */
