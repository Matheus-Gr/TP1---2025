#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include "tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Protótipos das funções
void InsereNaPaginaExterna(TipoApontadorEstrela Ap,
                           Registro Reg,
                           Estatisticas *estatisticas);

void InsereNaPaginaInterna(TipoApontadorEstrela Ap,
                           TipoChave Reg, 
                           TipoApontadorEstrela ApDir, 
                           Estatisticas *estatisticas);

void Ins_b_estrela(Registro Reg,
                   TipoApontadorEstrela Ap,
                   short *cresceu,
                   TipoChave *RegRetorno,
                   TipoApontadorEstrela *ApRetorno,
                   Estatisticas *estatisticas);
                   
bool Pesquisa(Registro *x,
              TipoApontadorEstrela Ap,
              Estatisticas *estatisticas,
              bool debug);

void Insere_b_estrela(Registro Reg,
                      TipoApontadorEstrela *Ap,
                      Estatisticas *estatisticas);

void inicializa_b_estrela(TipoApontadorEstrela *Arvore);

TipoApontadorEstrela ConstruirArvoreBStar(char *nomeArquivo, 
                                          int quantidade,
                                          Estatisticas *estatisticas,
                                          bool debug);

void liberaArvoreBEstrela(TipoApontadorEstrela Arvore);

void exibirItensArvoreBEstrela(Registro *registros, int quantidade);

void printArvoreBEstrela(TipoApontadorEstrela Arvore, int nivel);

#endif