#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

// Definição de Registro
typedef struct {
    int chave;              // Chave de pesquisa
    long dado1;             // Dado 1 (inteiro longo)
    char dado2[5000];       // Dado 2 (cadeia de 5000 caracteres)
} Registro;

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
    int profundidadeAB;
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

#define M 2
#define MM 2 * M

typedef struct Pagina* Apontador;

typedef struct Pagina {
    short nFilhos;
    Registro registros[MM];
    Apontador ponteiros[MM + 1];
} Pagina;

#define MB 3
#define MMB (2 * MB)
#define MB2 3
#define MMB2 (2 * MB2)

typedef int TipoChave;

typedef enum { Interna, Externa } TipoPagina;

typedef struct PaginaEstrela {
    TipoPagina Pt;
    union {
        struct {
            int ni; // Número de chaves na página interna
            TipoChave ri[MMB]; // Array de chaves
            struct PaginaEstrela *pi[MMB + 1]; // Ponteiros para páginas filhas
        } U0;
        struct {
            int ne; // Número de registros na página externa
            Registro re[MMB2]; // Array de registros
        } U1;
    } UU;
} PaginaEstrela;

typedef PaginaEstrela *ApontadorEstrela;

// Função para exibir um registro
void lerRegistro(Registro* registro);

#endif