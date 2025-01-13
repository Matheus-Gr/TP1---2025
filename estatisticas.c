#include <time.h>  // Para clock_t
#include "tipos.h"
#include <stdio.h>

void inicializarEstatisticas(Estatisticas *estatisticas) {
    estatisticas->transferencias = 0;
    estatisticas->comparacoes = 0;
    estatisticas->inicio = clock();
    estatisticas->tempoExecucao = 0.0;
}

void finalizarEstatisticas(Estatisticas *estatisticas) {
    estatisticas->tempoExecucao = (double)(clock() - estatisticas->inicio) * 1000 / CLOCKS_PER_SEC;

    printf("Transferencias: %d\n", estatisticas->transferencias);
    printf("Comparacoes: %d\n", estatisticas->comparacoes);
    printf("Tempo de execucao: %.2f ms\n", estatisticas->tempoExecucao);
}