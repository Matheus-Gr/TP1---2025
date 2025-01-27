#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h" 


void criarArvore(Registro *registros, int quantidade, const char* nomeArquivoArvore, Estatisticas *estatisticas, int debug);

int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug);

void atualizaPonteiros(FILE* arquivoArvore, NoBinario* itemInserir, Estatisticas *estatisticas);

void lerArvore(const char* nomeArquivoArvore);

#endif
