#include "indexado.h"
#include "tipos.h"
#include <stdlib.h>

#define ITENSPAGINA 6

int pesquisaIndexada(
    Registro* registro,
    int tamanho, 
    FILE* arquivo, 
    Estatisticas* estatisticas,
    int debug) {
    int nPaginas = (tamanho + ITENSPAGINA - 1) / ITENSPAGINA;

    Indice* tabelaIndices = malloc(nPaginas * sizeof(Indice));
    if (tabelaIndices == NULL) {
        perror("Erro ao alocar memória para tabela de índices");
        return 0;
    }

    Registro tempoRegistro;

    for (int i = 0; i < nPaginas; i++) {
        fseek(arquivo, i * ITENSPAGINA * sizeof(Registro), SEEK_SET);
        estatisticas->transferencias++;
        fread(&tempoRegistro, sizeof(Registro), 1, arquivo);
        
        tabelaIndices[i].posicao = i * ITENSPAGINA;
        tabelaIndices[i].chave = tempoRegistro.chave;
    }

    if (debug) {
        printf("Numero de paginas: %d\n", nPaginas);
        printf("Tabela de Indices:\n");
        printf("Pagina\tPosicao\tChave\n");
        for (int i = 0; i < nPaginas; i++) {
            printf("%d\t%d\t%d\n", i+ 1, tabelaIndices[i].posicao, tabelaIndices[i].chave);
        }
    }
    
    int pagina = -1;
    for (int i = 0; i < nPaginas; i++) {
        estatisticas->comparacoes++;
        if (tabelaIndices[i].chave > registro->chave) {
            break;
        }
        pagina = i + 1;
    }

    if(debug) printf("A chave %d esta na pagina %d.\n", registro->chave, pagina);

    


    return 1;
}
