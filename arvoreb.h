#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>

#include "tipos.h"

void inicializa(Apontador *arvore);

void Insere(Registro registro,
            Apontador *apontador,
            Estatisticas *estatisticas, int debug);

void InsereNaPagina (Apontador apontador,
                     Registro registro, 
                     Apontador apontador2, 
                     Estatisticas *estatisticas);

void Ins(Registro registro,
         Apontador apontador,
         short *cresceu,
         Registro *registro2, 
         Apontador *aporador2,
         Estatisticas *estatisticas,
         int debug);

int pesquisaArvoreB(Registro *registro,
                    Apontador apontador,
                    Estatisticas *estatisticas);

int calculaProfundidade(Apontador arvore);

#endif