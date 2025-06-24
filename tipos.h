/** @file tipos.h
 * @brief Definição de tipos e estruturas utilizadas no projeto.
 */

#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

/**
 * @brief Estrutura que representa um registro de dados.
 *
 * Contém uma chave de pesquisa, um campo inteiro longo e dois campos de texto.
 */
typedef struct
{
    int chave;        ///< Chave de pesquisa
    long dado1;       ///< Dado 1 (inteiro longo)
    char dado2[1000]; ///< Dado 2 (cadeia de 1000 caracteres)
    char dado3[5000]; ///< Dado 3 (cadeia de 5000 caracteres)
} Registro;

/**
 * @brief Estrutura para armazenar estatísticas de operações.
 *
 * Utilizada para medir transferências, comparações e tempos de execução
 * tanto na pesquisa quanto no pré-processamento.
 */
typedef struct
{
    int transferencias;     ///< Quantidade de transferências na pesquisa
    int comparacoes;        ///< Quantidade de comparações na pesquisa
    clock_t inicio;         ///< Tempo inicial da pesquisa
    double tempoExecucao;   ///< Tempo final de execução (ms)
    clock_t inicioPP;       ///< Tempo inicial do pré-processamento
    int transferenciasPP;   ///< Quantidade de transferências no pré-processamento
    int comparacoesPP;      ///< Quantidade de comparações no pré-processamento
    double tempoExecucaoPP; ///< Tempo final de pré-processamento (ms)
} Estatisticas;

/**
 * @brief Estrutura para índice de arquivos.
 *
 * Utilizada na pesquisa indexada para mapear posições e chaves.
 */
typedef struct
{
    int posicao; ///< Posição no arquivo
    int chave;   ///< Chave do registro
} Indice;

/**
 * @brief Estrutura de nó para árvore binária armazenada em arquivo.
 */
typedef struct
{
    Registro registro; ///< Registro armazenado no nó
    int esquerda;      ///< Índice do filho esquerdo
    int direita;       ///< Índice do filho direito
} NoBinario;

// Definições para Árvore B
#define M 2
#define MM 2 * M

typedef struct Pagina *Apontador;

/**
 * @brief Estrutura de página para árvore B.
 *
 * Contém registros e ponteiros para filhos.
 */
typedef struct Pagina
{
    short nFilhos;               ///< Número de registros na página
    Registro registros[MM];      ///< Vetor de registros
    Apontador ponteiros[MM + 1]; ///< Ponteiros para filhos
} Pagina;

// Definições para Árvore B*
#define MB 3
#define MMB (2 * MB)
#define MB2 3
#define MMB2 (2 * MB2)

typedef int TipoChave;

/**
 * @brief Enumeração para diferenciar páginas internas e externas na Árvore B*.
 */
typedef enum
{
    Interna, ///< Página interna (contém chaves e ponteiros)
    Externa  ///< Página externa (folha, contém registros)
} TipoPagina;

/**
 * @brief Estrutura de página para Árvore B*.
 *
 * Utiliza união para diferenciar páginas internas e externas.
 */
typedef struct PaginaEstrela
{
    TipoPagina Pt; ///< Tipo da página (interna ou externa)
    union
    {
        struct
        {
            int ni;                            ///< Número de chaves na página interna
            TipoChave ri[MMB];                 ///< Array de chaves
            struct PaginaEstrela *pi[MMB + 1]; ///< Ponteiros para páginas filhas
        } U0;
        struct
        {
            int ne;            ///< Número de registros na página externa
            Registro re[MMB2]; ///< Array de registros
        } U1;
    } UU;
} PaginaEstrela;

/**
 * @brief Ponteiro para página da Árvore B*.
 */
typedef PaginaEstrela *ApontadorEstrela;

/**
 * @brief Exibe um registro na saída padrão.
 *
 * @param registro Ponteiro para o registro a ser exibido.
 */
void lerRegistro(Registro *registro);

#endif