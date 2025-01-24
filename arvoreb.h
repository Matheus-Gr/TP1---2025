#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

void inicializa(Apontador *arvore);

void criaArvoreB(Apontador arvore,
                 FILE* arquivoEntrada,
                 Estatisticas *estatisticas);

void Insere(Registro registro,
            Apontador *apontador,
            Estatisticas *estatisticas);

void InsereNaPagina (Apontador apontador,
                     Registro registro, 
                     Apontador apontador2, 
                     Estatisticas *estatisticas);

void Ins(Registro registro,
         Apontador apontador,
         short *sla,
         Registro *registro2, 
         Apontador *aporador2,
         Estatisticas *estatisticas);

int pesquisaArvoreB(Registro *registro,
                    Apontador apontador,
                    Estatisticas *estatisticas);

#endif