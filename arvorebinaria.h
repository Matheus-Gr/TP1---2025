#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h" 


void criarArvore(FILE* arquivoEntrada, const char* nomeArquivoArvore);

int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug);

void atualizaPonteiros(FILE* arquivoArvore, NoBinario* itemInserir);

void lerArvore(const char* nomeArquivoArvore);

#endif
