#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include "tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Definições de constantes
#define MB 3
#define MMB (2 * MB)
#define MB2 3
#define MMB2 (2 * MB2)

typedef int TipoChave;

typedef enum { Interna, Externa } TipoPagina;

typedef struct TipoPaginaEstrela {
    TipoPagina Pt;
    union {
        struct {
            int ni; // Número de chaves na página interna
            TipoChave ri[MMB]; // Array de chaves
            struct TipoPaginaEstrela *pi[MMB + 1]; // Ponteiros para páginas filhas
        } U0;
        struct {
            int ne; // Número de registros na página externa
            TipoRegistro re[MMB2]; // Array de registros
        } U1;
    } UU;
} TipoPaginaEstrela;

typedef TipoPaginaEstrela *TipoApontadorEstrela;

// Protótipos das funções
void InsereNaPaginaExterna(TipoApontadorEstrela Ap, TipoRegistro Reg, Estatisticas *estatisticas);
void InsereNaPaginaInterna(TipoApontadorEstrela Ap, TipoChave Reg, TipoApontadorEstrela ApDir, Estatisticas *estatisticas);
void Ins_b_estrela(TipoRegistro Reg, TipoApontadorEstrela Ap, short *cresceu, TipoChave *RegRetorno, TipoApontadorEstrela *ApRetorno, Estatisticas *estatisticas);
bool Pesquisa(TipoRegistro *x, TipoApontadorEstrela Ap, Estatisticas *estatisticas, bool debug);
void Insere_b_estrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, Estatisticas *estatisticas);
void inicializa_b_estrela(TipoApontadorEstrela *Arvore);
TipoApontadorEstrela ConstruirArvoreBStar(char *nomeArquivo, int quantidade, Estatisticas *estatisticas, bool debug);
void liberaArvoreBEstrela(TipoApontadorEstrela Arvore);
void exibirItensArvoreBEstrela(TipoRegistro *registros, int quantidade);
void printArvoreBEstrela(TipoApontadorEstrela Arvore, int nivel);

#endif