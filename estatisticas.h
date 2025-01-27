#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "tipos.h"
#include <stdio.h>

void inicializarEstatisticas(Estatisticas *estatisticas);

void inicializarTimerPesquisa(Estatisticas *estatisticas);

void finalizarPreProcessamento(Estatisticas *estatisticas);

void finalizarEstatisticasPequisa(Estatisticas *estatisticas);

void printarEstatisticas(const Estatisticas *estatisticas);

#endif