#ifndef INDEXADO_H
#define INDEXADO_H

#include "tipos.h"
#include <stdbool.h>
#include <stdio.h>

int pesquisaIndexada(
    Registro* registro,
    int tamanho, 
    int ordem,
    FILE* arquivo,
    Estatisticas* estatisticas,
    int debug);

    
#endif
