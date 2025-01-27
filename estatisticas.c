#include <time.h>  // Para clock_t
#include "tipos.h"
#include <stdio.h>

void inicializarEstatisticas(Estatisticas *estatisticas) {
    estatisticas->transferencias = 0;
    estatisticas->comparacoes = 0;
    estatisticas->tempoExecucao = 0.0;
    estatisticas->transferenciasPP = 0;
    estatisticas->comparacoesPP = 0;
    estatisticas->tempoExecucaoPP = 0;
    estatisticas->inicioPP = clock();
}

void inicializarTimerPesquisa(Estatisticas *estatisticas){
    estatisticas->inicio = clock();
}

void finalizarPreProcessamento(Estatisticas *estatisticas) {
    estatisticas->tempoExecucaoPP = (double)(clock() - estatisticas->inicioPP) * 1000 / CLOCKS_PER_SEC;
}

void finalizarEstatisticasPequisa(Estatisticas *estatisticas){
    estatisticas->tempoExecucao = (double)(clock() - estatisticas->inicio) * 1000 / CLOCKS_PER_SEC;
}

void printarEstatisticas(const Estatisticas *estatisticas) {
    printf("=== Estatisticas de Pre-Processamento ===\n");
    printf("Transferencias (Pre-Processamento): %d\n", estatisticas->transferenciasPP);
    printf("Comparacoes (Pre-Processamento): %d\n", estatisticas->comparacoesPP);
    printf("Tempo de Execucao (Pre-Processamento): %.2f ms (%.2f s)\n", estatisticas->tempoExecucaoPP, estatisticas->tempoExecucaoPP / 1000.0);
    
    printf("=== Estatisticas ===\n");
    printf("Transferencias: %d\n", estatisticas->transferencias);
    printf("Comparacoes: %d\n", estatisticas->comparacoes);
    printf("Tempo de Execucao: %.2f ms\n", estatisticas->tempoExecucao);
}
