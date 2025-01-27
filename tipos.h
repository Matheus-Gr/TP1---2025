#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

// Definição de Registro
typedef struct {
    int chave;          // Chave de pesquisa
    long dado1;         // Dado 1 (inteiro longo)
    char dado2[5000];   // Dado 2 (cadeia de caracteres)
} Registro;

// TipoRegistro é um alias para Registro
typedef Registro TipoRegistro;

// Definição de Estatisticas
typedef struct {
    int transferencias;
    int comparacoes;
    clock_t inicio;
    double tempoExecucao;
    double tempoPreProcessamento; // Novo campo
} Estatisticas;

// Definição de Indice
typedef struct {
    int posicao;        // Posição no arquivo
    int chave;          // Chave do registro
} Indice;

// Definição de NoBinario
typedef struct {
    Registro registro;  // Registro armazenado no nó
    int esquerda;       // Índice do filho esquerdo
    int direita;        // Índice do filho direito
} NoBinario;

// Função para exibir um registro
void lerRegistro(Registro* registro);

#endif // TIPOS_H