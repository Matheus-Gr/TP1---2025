/**
 * @file arvorebinaria.h
 * @brief Definição da estrutura da árvore binária e funções relacionadas à manipulação da árvore.
 */

#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

/**
 * @brief Atualiza os ponteiros da árvore binária após a inserção de um novo nó.
 *
 * Percorre a árvore binária armazenada em arquivo para encontrar o nó pai do novo nó inserido,
 * e atualiza o ponteiro esquerdo ou direito do nó pai para apontar para o novo nó.
 * Atualiza as estatísticas de pré-processamento.
 *
 * @param arquivoArvore Ponteiro para o arquivo da árvore binária.
 * @param itemInserir Ponteiro para o nó recém-inserido.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 */
void atualizaPonteiros(FILE *arquivoArvore, NoBinario *itemInserir, Estatisticas *estatisticas);

/**
 * @brief Cria uma árvore binária a partir de um vetor de registros.
 *
 * Insere sequencialmente cada registro do vetor na árvore binária, armazenando os nós em arquivo.
 * Atualiza as estatísticas de pré-processamento e pode exibir informações de depuração.
 *
 * @param registros Vetor de registros a serem inseridos.
 * @param quantidade Quantidade de registros no vetor.
 * @param nomeArquivoArvore Nome do arquivo onde a árvore será armazenada.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 * @param debug Se diferente de zero, exibe informações de depuração.
 */
void criarArvore(Registro *registros, int quantidade, const char *nomeArquivoArvore, Estatisticas *estatisticas, int debug);

/**
 * @brief Busca um registro na árvore binária armazenada em arquivo.
 *
 * Percorre a árvore binária em arquivo a partir da raiz, comparando a chave do registro buscado
 * com as chaves dos nós, até encontrar o registro ou atingir uma folha.
 * Atualiza as estatísticas de pesquisa.
 *
 * @param nomeArquivoArvore Nome do arquivo da árvore binária.
 * @param registro Ponteiro para o registro a ser buscado (chave deve estar preenchida).
 *                 Se encontrado, os demais campos do registro são preenchidos.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 * @param debug Se diferente de zero, exibe informações de depuração.
 * @return 1 se o registro foi encontrado, 0 caso contrário.
 */
int buscarArvore(const char *nomeArquivoArvore, Registro *registro, Estatisticas *estatisticas, int debug);

/**
 * @brief Lê e imprime o conteúdo da árvore binária armazenada em arquivo.
 *
 * Percorre todos os nós da árvore binária no arquivo e imprime suas informações (posição, chave, ponteiros).
 *
 * @param nomeArquivoArvore Nome do arquivo da árvore binária.
 */
void lerArvore(const char *nomeArquivoArvore);

#endif
