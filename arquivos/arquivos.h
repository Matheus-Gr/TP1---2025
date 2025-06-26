/** @file arquivos.h
 *  @brief Funções para criar ou ler arquivos binários.
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../tipos.h"

/**
 * @brief Lê um vetor de índices inteiros de um arquivo binário.
 *
 * O arquivo deve conter exatamente quantidadeRegistros inteiros.
 *
 * @param nomeArquivo Caminho do arquivo de índices.
 * @param indices Vetor onde os índices lidos serão armazenados.
 * @param quantidadeRegistros Quantidade de índices a serem lidos.
 */
void lerIndicesDoArquivo(const char *nomeArquivo, int *indices, int quantidadeRegistros);

/**
 * @brief Gera um arquivo binário de registros, ordenado, reverso ou aleatório.
 *
 * O arquivo gerado terá quantidadeRegistros registros do tipo Registro.
 * Se situacao == 1, gera registros ordenados crescentes.
 * Se situacao == 2, gera registros ordenados decrescentes.
 * Se situacao == 3, usa um arquivo de índices para ordem aleatória.
 *
 * @param caminhoArquivo Caminho do arquivo binário a ser gerado.
 * @param quantidadeRegistros Quantidade de registros a serem gerados.
 * @param situacao 1 (ordenado), 2 (reverso), 3 (aleatório).
 * @param arquivoIndices Caminho do arquivo de índices aleatórios (usado se situacao == 3).
 */
void gerarArquivoBinario(const char *caminhoArquivo, int quantidadeRegistros, int situacao, const char *arquivoIndices);

/**
 * @brief Lê e imprime todos os registros de um arquivo binário.
 *
 * Exibe os campos de cada registro na saída padrão.
 *
 * @param caminhoArquivo Caminho do arquivo binário a ser lido.
 */
void lerArquivoBinario(const char *caminhoArquivo);

#endif
