#include "indexado.h"
#include "tipos.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ITENSPAGINA 6 // Número de registros por página na busca indexada.

// Função de pré-processamento para criar uma tabela de índices.
Indice* preProcessarIndices(FILE* arquivo, int tamanho, int* nPaginas, Estatisticas* estatisticas, double* tempoPreProcessamento) {
    clock_t inicio = clock(); // Marca o início do pré-processamento.

    *nPaginas = (tamanho + ITENSPAGINA - 1) / ITENSPAGINA;

    Indice* tabelaIndices = malloc(*nPaginas * sizeof(Indice));
    if (tabelaIndices == NULL) {
        perror("Erro ao alocar memória para tabela de índices");
        return NULL;
    }

    Registro tempoRegistro;
    for (int i = 0; i < *nPaginas; i++) {
        fseek(arquivo, i * ITENSPAGINA * sizeof(Registro), SEEK_SET);
        estatisticas->transferencias++;
        fread(&tempoRegistro, sizeof(Registro), 1, arquivo);

        tabelaIndices[i].posicao = i * ITENSPAGINA;
        tabelaIndices[i].chave = tempoRegistro.chave;
    }

    clock_t fim = clock(); // Marca o fim do pré-processamento.
    *tempoPreProcessamento = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000; // Converte para milissegundos.

    return tabelaIndices;
}

// Realiza uma pesquisa indexada em um arquivo de registros.
int pesquisaIndexada(
    Registro* registro,
    int tamanho,
    int ordem,
    FILE* arquivo,
    Estatisticas* estatisticas,
    int debug
) {
    int nPaginas;
    double tempoPreProcessamento = 0.0;

    // Pré-processa os índices antes de realizar a busca.
    Indice* tabelaIndices = preProcessarIndices(arquivo, tamanho, &nPaginas, estatisticas, &tempoPreProcessamento);
    if (tabelaIndices == NULL) {
        return 0;
    }

    printf("Tempo de pré-processamento: %.2f ms\n", tempoPreProcessamento);

    if (debug) {
        printf("Numero de paginas: %d\n", nPaginas);
        printf("Tabela de Indices:\n");
        printf("Pagina\tPosicao\tChave\n");
        for (int i = 0; i < nPaginas; i++) {
            printf("%d\t%d\t%d\n", i + 1, tabelaIndices[i].posicao, tabelaIndices[i].chave);
        }
    }

    int pagina = -1;

    // Encontra a página que possivelmente contém o registro desejado.
    for (int i = 0; i < nPaginas; i++) {
        estatisticas->comparacoes++;
        if ((ordem == 1 && tabelaIndices[i].chave > registro->chave) ||
            (ordem == 2 && tabelaIndices[i].chave < registro->chave)) {
            break;
        }
        pagina = i + 1;
    }

    if (debug) {
        printf("A chave %d esta na pagina %d.\n", registro->chave, pagina);
    }

    if (pagina == -1) {
        free(tabelaIndices);
        return 0;
    }

    // Busca sequencial na página encontrada.
    fseek(arquivo, tabelaIndices[pagina - 1].posicao * sizeof(Registro), SEEK_SET);
    Registro tempoRegistro;
    for (int i = 0; i < ITENSPAGINA; i++) {
        estatisticas->transferencias++;
        if (fread(&tempoRegistro, sizeof(Registro), 1, arquivo) != 1) {
            break; 
        }
        estatisticas->comparacoes++;
        if (tempoRegistro.chave == registro->chave) {
            *registro = tempoRegistro;
            free(tabelaIndices);
            return 1;
        }
    }

    free(tabelaIndices);
    return 0;
}
