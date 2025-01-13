#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../tipos.h"

void gerarArquivoBinario(const char *caminhoArquivo, int quantidadeRegistros, int situacao, const char *arquivoIndices);
void lerArquivoBinario(const char *caminhoArquivo);

#endif
