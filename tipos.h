#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

// Definição de Registro
typedef struct {
    int chave;              // Chave de pesquisa
    long dado1;             // Dado 1 (inteiro longo)
    char dado2[5000];       // Dado 2 (cadeia de 5000 caracteres)
} Registro;

// TipoRegistro é um alias para Registro
typedef Registro TipoRegistro;

// Definição de Estatisticas
typedef struct {
    int transferencias;     // Quantidade de transfericias na pesquisa
    int comparacoes;        // Quantidade de comparações na pesquisa
    clock_t inicio;         // Tempo inicial da pesquisa
    double tempoExecucao;   // Tempo final de execução (ms)
    clock_t inicioPP;       // Tempo inicial do pré-processamento
    int transferenciasPP;   // Quantidade de transfericias no pré-processamento
    int comparacoesPP;      // Quantidade de comparações no pré-processamento
    double tempoExecucaoPP; // Tempo final de pré-processamento (ms)
} Estatisticas;

// Definição de Indice
typedef struct {
    int posicao;            // Posição no arquivo
    int chave;              // Chave do registro
} Indice;

// Definição de NoBinario
typedef struct {
    Registro registro;      // Registro armazenado no nó
    int esquerda;           // Índice do filho esquerdo
    int direita;            // Índice do filho direito
} NoBinario;

// Função para exibir um registro
void lerRegistro(Registro* registro);

#endif