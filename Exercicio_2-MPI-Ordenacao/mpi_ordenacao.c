#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

/**
 * Compilar: "mpicc mpi_ordenacao.c -o mpi_ordenacao -Wall"
 * Executar: "mpirun -np «numeroprocessos» ./mpi_ordenacao «pposicoes»"
 */

 int main(int argc, char **argv) {
     int size, rank;

     MPI_Init(&argc, &argv);

     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

     MPI_Finalize();

     return 0;
 }


/* ETAPAS:
 * - [ ] O Root criar um vetor V de P posicoes preenchido com numeros aleatorios
 *       inteiros entre 0 e 100 e imprimir o vetor gerado.
 * - [ ] Distribuir o vetor em N processos, incluindo si proprio.
 * - [ ] Todos os processos ordenar seus vetores localmente, para facilitar a
 *       ordenacao global.
 * - [ ] O processo Root imprimir o vetor ordenado.
 * - [ ] Todos os processos devem finalizar.
 */
