#include "tipos.h"
#include <stdio.h>

void lerRegistro(Registro* registro) {
    printf("  Chave: %d\n", registro->chave);
    printf("  Dado1: %ld\n", registro->dado1);
    printf("  Dado2: %s\n", registro->dado2);
}