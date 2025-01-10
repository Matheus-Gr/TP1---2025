#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

typedef struct {
    int chave;
    long dado1;
    char dado2[5000];
} Registro;

typedef struct {
    int transferencias;  
    int comparacoes; 
    clock_t inicio;
    double tempoExecucao; 
} Estatisticas;

typedef struct {
    int chaveEncontrada; 
    Registro registro; 
    Estatisticas estatisticas; 
} Resposta;

typedef struct {
    int posicao;
    int chave;
} Indice;

void lerRegistro(Registro* registro);

#endif
