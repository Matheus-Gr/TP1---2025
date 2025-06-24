/** @file estatisticas.h
 * @brief Definição da estrutura Estatisticas e funções relacionadas à coleta de estatísticas.
 */

#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "tipos.h"
#include <stdio.h>

/**
 * @brief Inicializa todos os campos da estrutura Estatisticas com zero e inicia o timer do pré-processamento.
 * @param estatisticas Ponteiro para a estrutura Estatisticas a ser inicializada.
 */
void inicializarEstatisticas(Estatisticas *estatisticas);

/**
 * @brief Inicia o timer para medir o tempo de execução da pesquisa.
 * @param estatisticas Ponteiro para a estrutura Estatisticas.
 */
void inicializarTimerPesquisa(Estatisticas *estatisticas);

/**
 * @brief Finaliza o tempo de pré-processamento, calculando o tempo decorrido desde o início.
 * @param estatisticas Ponteiro para a estrutura Estatisticas.
 */
void finalizarPreProcessamento(Estatisticas *estatisticas);

/**
 * @brief Finaliza o tempo de execução da pesquisa, calculando o tempo decorrido desde o início.
 * @param estatisticas Ponteiro para a estrutura Estatisticas.
 */
void finalizarEstatisticasPequisa(Estatisticas *estatisticas);

/**
 * @brief Imprime na tela as estatísticas de pré-processamento e de pesquisa.
 * @param estatisticas Ponteiro constante para a estrutura Estatisticas.
 */
void printarEstatisticas(const Estatisticas *estatisticas);

/**
 * @brief Salva as estatísticas em um arquivo texto, adicionando uma nova linha com os dados do experimento.
 *
 * O formato da linha é:
 * {metodo}-{quantidade}-{ordem}-{chave}-{transferenciasPP}-{comparacoesPP}-{tempoExecucaoPP}-{transferencias}-{comparacoes}-{tempoExecucao}\n
 *
 * @param metodo Método de pesquisa utilizado.
 * @param quantidade Quantidade de registros.
 * @param ordem Situação de ordem do arquivo.
 * @param chave Chave pesquisada.
 * @param estatisticas Ponteiro constante para a estrutura Estatisticas.
 * @param caminhoArquivo Caminho do arquivo onde as estatísticas serão salvas.
 */
void salvarEstatisticas(int metodo, int quantidade, int ordem, int chave, const Estatisticas *estatisticas, const char *caminhoArquivo);

#endif