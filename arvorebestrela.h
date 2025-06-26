/**
 * @file arvorebestrela.h
 * @brief Definicao da estrutura da arvore B* e funcoes relacionadas a manipulacao da arvore.
 */

#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include "tipos.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Inicializa a raiz da arvore B* como NULL.
 * @param Arvore Ponteiro para a raiz da arvore.
 */
void inicializa_b_estrela(ApontadorEstrela *Arvore);

/**
 * @brief Busca um registro na arvore B*.
 * @param x Ponteiro para o registro a ser buscado (chave deve estar preenchida).
 * @param Ap Ponteiro para a raiz da arvore.
 * @param estatisticas Ponteiro para a estrutura de estatisticas.
 * @param debug Se diferente de zero, imprime informacoes de depuracao.
 * @return 1 se o registro foi encontrado, 0 caso contrario.
 */
int pesquisaBEstrela(Registro *x,
                     ApontadorEstrela Ap,
                     Estatisticas *estatisticas,
                     int debug);

/**
 * @brief Insere um registro em uma folha nao cheia da arvore B*.
 * @param Ap Pagina externa (folha).
 * @param Reg Registro a ser inserido.
 * @param estatisticas Ponteiro para a estrutura de estatisticas.
 */
void InsereNaPaginaExterna(ApontadorEstrela Ap,
                           Registro Reg,
                           Estatisticas *estatisticas);

/**
 * @brief Insere uma chave em uma pagina interna nao cheia da arvore B*.
 * @param Ap Pagina interna.
 * @param Reg Chave a ser inserida.
 * @param ApDir Ponteiro para o filho direito.
 * @param estatisticas Ponteiro para a estrutura de estatisticas.
 */
void InsereNaPaginaInterna(ApontadorEstrela Ap,
                           TipoChave Reg,
                           ApontadorEstrela ApDir,
                           Estatisticas *estatisticas);

/**
 * @brief Funcao recursiva de insercao na arvore B* (com splits otimizados).
 * @param Reg Registro a ser inserido.
 * @param Ap Pagina atual.
 * @param cresceu Flag de crescimento da arvore.
 * @param RegRetorno Chave promovida apos split.
 * @param ApRetorno Nova pagina apos split.
 * @param estatisticas Ponteiro para a estrutura de estatisticas.
 */
void Ins_b_estrela(Registro Reg,
                   ApontadorEstrela Ap,
                   short *cresceu,
                   TipoChave *RegRetorno,
                   ApontadorEstrela *ApRetorno,
                   Estatisticas *estatisticas);

/**
 * @brief Funcao principal de insercao na arvore B*.
 *        Cria a raiz se a arvore estiver vazia.
 * @param Reg Registro a ser inserido.
 * @param Ap Ponteiro para a raiz da arvore.
 * @param estatisticas Ponteiro para a estrutura de estatisticas.
 * @param debug Se diferente de zero, imprime informacoes de depuracao.
 */
void Insere_b_estrela(Registro Reg,
                      ApontadorEstrela *Ap,
                      Estatisticas *estatisticas,
                      int debug);

/**
 * @brief Libera toda a memoria alocada para a arvore B*.
 * @param Arvore Raiz da arvore.
 */
void liberaArvoreBEstrela(ApontadorEstrela Arvore);

/**
 * @brief Exibe a estrutura da arvore B* de forma hierarquica (para depuracao).
 * @param Arvore Raiz da arvore.
 * @param nivel Profundidade atual (para indentacao).
 */
void printArvoreBEstrela(ApontadorEstrela Arvore, int nivel);

/**
 * @brief Calcula a profundidade (altura) da arvore B*.
 * @param Arvore Raiz da arvore.
 * @return Profundidade da arvore (0 se vazia).
 */
int calculaProfundidadeBEstrela(ApontadorEstrela Arvore);

#endif