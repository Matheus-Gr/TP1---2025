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
    int posicao;
    int chave;
} Indice;

typedef struct {
    Registro registro;
    int esquerda;
    int direita;
} NoBinario;

void lerRegistro(Registro* registro);

#endif
