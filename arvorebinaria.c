#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "tipos.h"

// Atualiza os ponteiros da árvore binária após a inserção de um novo nó
void atualizaPonteiros(FILE* arquivoArvore, NoBinario* itemInserir, Estatisticas *estatisticas) {
    // Calcula a quantidade de itens no arquivo da árvore
    _fseeki64(arquivoArvore, 0, SEEK_END);
    int64_t qtdItensArquivo = _ftelli64(arquivoArvore) / sizeof(NoBinario);
    _fseeki64(arquivoArvore, 0, SEEK_SET);

    // Caso haja apenas um nó, não há ponteiros a serem atualizados
    estatisticas->comparacoesPP++;
    if (qtdItensArquivo == 1) return;

    NoBinario lido;
    int64_t ponteiro = 1; // Ponteiro inicial para o nó raiz
    int64_t desloc;

    // Percorre a árvore até encontrar uma posição para inserir o novo nó
    do {
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        if (_fseeki64(arquivoArvore, desloc, SEEK_SET) != 0) {
            perror("Erro ao posicionar o cursor no arquivo");
            exit(1);
        }
        estatisticas->transferenciasPP++;
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);

        // Decide para qual lado (esquerda ou direita) seguir
        estatisticas->comparacoesPP++;
        ponteiro = (itemInserir->registro.chave > lido.registro.chave) ? lido.direita : lido.esquerda;
    } while (ponteiro != -1);

    // Retorna para o nó anterior
    _fseeki64(arquivoArvore, (int64_t) -(sizeof(NoBinario)), SEEK_CUR);

    // Atualiza o ponteiro do nó pai para apontar para o novo nó
    estatisticas->comparacoesPP++;
    if (itemInserir->registro.chave > lido.registro.chave)
        lido.direita = qtdItensArquivo;
    else
        lido.esquerda = qtdItensArquivo;

    // Escreve o nó atualizado no arquivo
    fwrite(&lido, sizeof(NoBinario), 1, arquivoArvore);
    return;
}

// Cria uma árvore binária a partir de um arquivo de entrada
void criarArvore(Registro *registros,
                 int quantidade,
                 const char* nomeArquivoArvore,
                 Estatisticas *estatisticas, int debug) {

    // Abre o arquivo da árvore binária para escrita
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "wb+");
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da arvore binaria");
        exit(1);
    }

    int itensinseridos = 0;
    clock_t inicio = clock();
    double tempoInsercao = 0.0;

    for (int i = 0; i < quantidade; i++) {
        estatisticas->transferenciasPP++;
        NoBinario no;
        no.registro = registros[i];
        no.esquerda = -1;
        no.direita = -1;

        // Insere o nó no final do arquivo
        _fseeki64(arquivoArvore, 0, SEEK_END);
        if (fwrite(&no, sizeof(NoBinario), 1, arquivoArvore) != 1) {
            perror("Erro ao escrever no arquivo");
            exit(1);
        }

        if(debug && (itensinseridos % 50000 == 0)){
            tempoInsercao = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC;
            printf("Itens inseridos %d, tempo de insercao do lote %0.2f ms\n", itensinseridos, tempoInsercao);
            inicio = clock();
        }

        // Atualiza os ponteiros da árvore para incluir o novo nó
        atualizaPonteiros(arquivoArvore, &no, estatisticas);
        itensinseridos++;
    }

    fclose(arquivoArvore);
    if (debug){
        printf("Arvore binaria criada com sucesso no arquivo: %s\n", nomeArquivoArvore);
    }
}

// Busca um registro na árvore binária
int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug) {
    // Abre o arquivo da árvore binária para leitura
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da arvore binária");
        exit(1);
    }

    NoBinario lido;
    int64_t  ponteiro = 1; // Começa a busca pela raiz
    long desloc;

    // Percorre a árvore até encontrar o registro ou atingir uma folha
    do {
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        _fseeki64(arquivoArvore, desloc, SEEK_SET);
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);
        estatisticas->transferencias++;

        // Verifica se encontrou o registro
        estatisticas->comparacoes++;
        if (registro->chave == lido.registro.chave) {
            *registro = lido.registro;
            return 1; // Registro encontrado
        }

        // Decide para qual lado da árvore seguir
        estatisticas->comparacoes++;
        ponteiro = (registro->chave > lido.registro.chave) ? lido.direita : lido.esquerda;

    } while (ponteiro != -1);

    return 0; // Registro não encontrado
}

// Lê e imprime o conteúdo da árvore binária
void lerArvore(const char* nomeArquivoArvore) {
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da arvore binaria");
        return;
    }

    NoBinario no;
    int posicao = 0;

    // Imprime os cabeçalhos
    printf("Posicao | Chave | Esquerda | Direita\n");
    printf("----------------------------------\n");

    // Lê e imprime cada nó da árvore
    while (fread(&no, sizeof(NoBinario), 1, arquivoArvore) == 1) {
        printf("%8d | %5d | %8d | %6d\n", posicao, no.registro.chave, no.esquerda, no.direita);
        posicao++;
    }

    fclose(arquivoArvore);
}
