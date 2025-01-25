#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tipos.h"

// Atualiza os ponteiros da árvore binária após a inserção de um novo nó
void atualizaPonteiros(FILE* arquivoArvore, NoBinario* itemInserir) {
    // Calcula a quantidade de itens no arquivo da árvore
    fseek(arquivoArvore, 0, SEEK_END);
    long qtdItensArquivo = ftell(arquivoArvore) / sizeof(NoBinario);
    fseek(arquivoArvore, 0, SEEK_SET);

    // Caso haja apenas um nó, não há ponteiros a serem atualizados
    if (qtdItensArquivo == 1) return;

    NoBinario lido;
    long ponteiro = 1; // Ponteiro inicial para o nó raiz
    long desloc;

    // Percorre a árvore até encontrar uma posição para inserir o novo nó
    do {
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        fseek(arquivoArvore, desloc, SEEK_SET);
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);

        // Decide para qual lado (esquerda ou direita) seguir
        ponteiro = (itemInserir->registro.chave > lido.registro.chave) ? lido.direita : lido.esquerda;
    } while (ponteiro != -1);

    // Retorna para o nó anterior
    fseek(arquivoArvore, -(sizeof(NoBinario)), SEEK_CUR);

    // Atualiza o ponteiro do nó pai para apontar para o novo nó
    if (itemInserir->registro.chave > lido.registro.chave)
        lido.direita = qtdItensArquivo;
    else
        lido.esquerda = qtdItensArquivo;

    // Escreve o nó atualizado no arquivo
    fwrite(&lido, sizeof(NoBinario), 1, arquivoArvore);
    return;
}

// Cria uma árvore binária a partir de um arquivo de entrada
void criarArvore(FILE* arquivoEntrada, const char* nomeArquivoArvore) {
    // Abre o arquivo da árvore binária para escrita
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "wb+");
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da arvore binaria");
        exit(1);
    }

    Registro lido;

    // Lê os registros do arquivo de entrada e insere na árvore
    while (fread(&lido, sizeof(Registro), 1, arquivoEntrada) == 1) {
        NoBinario no;
        no.registro = lido;
        no.esquerda = -1;
        no.direita = -1;

        // Insere o nó no final do arquivo
        fseek(arquivoArvore, 0, SEEK_END);
        if (fwrite(&no, sizeof(NoBinario), 1, arquivoArvore) != 1) {
            perror("Erro ao escrever no arquivo");
            exit(1);
        }

        // Atualiza os ponteiros da árvore para incluir o novo nó
        atualizaPonteiros(arquivoArvore, &no);
    }

    fclose(arquivoArvore);
    printf("Arvore binaria criada com sucesso no arquivo: %s\n", nomeArquivoArvore);
}

// Calcula o tempo de pré-processamento (criação da árvore binária)
double calcularTempoPreProcessamento(FILE* arquivoEntrada, const char* nomeArquivoArvore) {
    clock_t inicio, fim;
    double tempoGasto;

    // Inicia a contagem de tempo
    inicio = clock();

    // Cria a árvore binária
    criarArvore(arquivoEntrada, nomeArquivoArvore);

    // Finaliza a contagem de tempo
    fim = clock();

    // Calcula o tempo gasto em milissegundos
    tempoGasto = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    //printf("tempo de pre processamento: %.2lf ms\n", tempoGasto);
    return tempoGasto;
}

// Busca um registro na árvore binária
int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug) {
    // Abre o arquivo da árvore binária para leitura
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da árvore binária");
        exit(1);
    }

    NoBinario lido;
    long ponteiro = 1; // Começa a busca pela raiz
    long desloc;

    // Percorre a árvore até encontrar o registro ou atingir uma folha
    do {
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        fseek(arquivoArvore, desloc, SEEK_SET);
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);
        estatisticas->transferencias += 1;

        // Verifica se encontrou o registro
        if (registro->chave == lido.registro.chave) {
            estatisticas->comparacoes += 1;
            *registro = lido.registro;
            return 1; // Registro encontrado
        }

        // Decide para qual lado da árvore seguir
        ponteiro = (registro->chave > lido.registro.chave) ? lido.direita : lido.esquerda;
        estatisticas->comparacoes += 1;

    } while (ponteiro != -1);

    return 0; // Registro não encontrado
}

// Lê e imprime o conteúdo da árvore binária
void lerArvore(const char* nomeArquivoArvore) {
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da árvore binária");
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
