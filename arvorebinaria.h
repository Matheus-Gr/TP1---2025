#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h" 


int inserirArvore(FILE* arquivoArvore, int posicaoAtual, Registro novoRegistro);

void criarArvore(FILE* arquivoEntrada, const char* nomeArquivoArvore);

int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug);

#endif
