#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include "tipos.h"
#include <stdio.h>
#include <stdlib.h>

// Protótipos das funções
void InsereNaPaginaExterna(ApontadorEstrela Ap,
                           Registro Reg,
                           Estatisticas *estatisticas);

void InsereNaPaginaInterna(ApontadorEstrela Ap,
                           TipoChave Reg, 
                           ApontadorEstrela ApDir, 
                           Estatisticas *estatisticas);

void Ins_b_estrela(Registro Reg,
                   ApontadorEstrela Ap,
                   short *cresceu,
                   TipoChave *RegRetorno,
                   ApontadorEstrela *ApRetorno,
                   Estatisticas *estatisticas);
                   
int pesquisaBEstrela(Registro *x,
              ApontadorEstrela Ap,
              Estatisticas *estatisticas,
              int debug);

void Insere_b_estrela(Registro Reg,
                      ApontadorEstrela *Ap,
                      Estatisticas *estatisticas);

void inicializa_b_estrela(ApontadorEstrela *Arvore);

void liberaArvoreBEstrela(ApontadorEstrela Arvore);

void exibirItensArvoreBEstrela(Registro *registros, int quantidade);

void printArvoreBEstrela(ApontadorEstrela Arvore, int nivel);

#endif