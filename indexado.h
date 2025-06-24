/** @file indexado.h
 * @brief Definição da estrutura de índice e funções relacionadas à pesquisa indexada.
 */

#ifndef INDEXADO_H
#define INDEXADO_H

#include "tipos.h"
#include "estatisticas.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Realiza o pré-processamento para criar uma tabela de índices para pesquisa indexada.
 *
 * Cada índice aponta para o primeiro registro de cada página no arquivo.
 * Também atualiza as estatísticas de pré-processamento.
 *
 * @param arquivo Ponteiro para o arquivo binário de registros.
 * @param tamanho Quantidade total de registros no arquivo.
 * @param nPaginas Ponteiro para inteiro onde será armazenado o número de páginas calculado.
 * @param estatisticas Ponteiro para a estrutura de estatísticas a ser atualizada.
 * @return Ponteiro para o vetor de índices criado (deve ser liberado pelo chamador), ou NULL em caso de erro.
 */
Indice *preProcessarIndices(FILE *arquivo, int tamanho, int *nPaginas, Estatisticas *estatisticas);

/**
 * @brief Realiza uma pesquisa indexada em um arquivo de registros.
 *
 * Utiliza a tabela de índices para localizar rapidamente a página onde a chave pode estar,
 * e então faz uma busca sequencial dentro da página. Atualiza as estatísticas de pesquisa.
 *
 * @param registro Ponteiro para o registro a ser buscado (a chave deve estar preenchida).
 *                 Se encontrado, os demais campos do registro são preenchidos.
 * @param tamanho Quantidade total de registros no arquivo.
 * @param ordem Situação de ordem do arquivo (1 para crescente, 2 para decrescente).
 * @param arquivo Ponteiro para o arquivo binário de registros.
 * @param estatisticas Ponteiro para a estrutura de estatísticas a ser atualizada.
 * @param debug Se diferente de zero, imprime informações detalhadas do processo.
 * @return 1 se o registro foi encontrado, 0 caso contrário.
 */
int pesquisaIndexada(
    Registro *registro,
    int tamanho,
    int ordem,
    FILE *arquivo,
    Estatisticas *estatisticas,
    int debug);

#endif
