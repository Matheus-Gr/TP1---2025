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

#define M 4
#define MM 2 * M

typedef struct Pagina* Apontador;

typedef struct Pagina {
    short nFilhos;
    Registro registros[MM];
    Apontador ponteiros[MM + 1];
} Pagina;

void lerRegistro(Registro* registro);

#endif
