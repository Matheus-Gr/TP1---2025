/**
 * @file arvoreb.h
 * @brief Definição da estrutura da árvore B e funções relacionadas à manipulação da árvore.
 */

#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
#include "tipos.h"

/**
 * @brief Inicializa a raiz da árvore B como NULL.
 * @param arvore Ponteiro para a raiz da árvore.
 */
void inicializa(Apontador *arvore);

/**
 * @brief Função principal para inserir um registro na árvore B.
 * Gerencia o crescimento da raiz, se necessário.
 * @param registro Registro a ser inserido.
 * @param apontador Ponteiro para a raiz da árvore.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 * @param debug Ativa logs durante a inserção.
 */
void Insere(Registro registro,
            Apontador *apontador,
            Estatisticas *estatisticas, int debug);

/**
 * @brief Insere um registro em uma página não cheia da árvore B.
 * Reorganiza os elementos e ponteiros da página para manter a ordem.
 * @param apontador Página onde o registro será inserido.
 * @param registro Registro a ser inserido.
 * @param apontador2 Ponteiro para o filho direito do registro.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 */
void InsereNaPagina(Apontador apontador,
                    Registro registro,
                    Apontador apontador2,
                    Estatisticas *estatisticas);

/**
 * @brief Função auxiliar recursiva para inserção na árvore B.
 * Realiza splits de páginas quando necessário e promove registros.
 * @param registro Registro a ser inserido.
 * @param apontador Página atual na recursão.
 * @param cresceu Flag que indica se a árvore aumentou de altura.
 * @param registro2 Registro promovido após split.
 * @param apontador2 Nova página criada após split.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 * @param debug Ativa logs para depuração.
 */
void Ins(Registro registro,
         Apontador apontador,
         short *cresceu,
         Registro *registro2,
         Apontador *apontador2,
         Estatisticas *estatisticas,
         int debug);

/**
 * @brief Busca um registro pela chave na árvore B.
 * Percorre recursivamente as páginas da árvore B para localizar o registro.
 * @param registro Ponteiro para o registro com a chave buscada (resultado preenchido se encontrado).
 * @param apontador Página atual na recursão.
 * @param estatisticas Ponteiro para a estrutura de estatísticas.
 * @return 1 se encontrado, 0 caso contrário.
 */
int pesquisaArvoreB(Registro *registro,
                    Apontador apontador,
                    Estatisticas *estatisticas);

/**
 * @brief Calcula a altura (profundidade) da árvore B.
 * Desce pelo primeiro filho até chegar a uma folha.
 * @param arvore Raiz da árvore.
 * @return Profundidade da árvore (0 se vazia).
 */
int calculaProfundidade(Apontador arvore);

#endif