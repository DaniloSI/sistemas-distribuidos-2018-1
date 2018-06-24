RESULTADOS
    - Montecarlo Serial:
        - Valor do PI: 3.147477.
        - Tempo gasto: 3.387 segundos.
    - Montecarlo MPI:
        - Valor do PI: 3.146535.
        - Tempo gasto: 0.944 segundos.

OBSERVAÇÕES
    - Geração de valores aleatórios: Foi utilizado a função "rand()" e também
      a função gettimeofday do sys/time compatível com Linux.
    - Cálculo do tempo de execução: Foi utilizado a função "clock()" para
      calcular o tempo de execução.
    - Sistema utilizado para rodar os programas:
        - SO: Ubuntu 18.04 bionic
        - Kernel: x86_64 Linux 4.15.0-23-generic
        - CPU: Intel Core i5-3470 @ 4x 3.6GHz
        - GPU: GeForce GT 420
        - RAM: 3360MiB / 7922MiB
