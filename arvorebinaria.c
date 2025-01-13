#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

int inserirArvore(FILE* arquivoArvore, int posicaoAtual, Registro novoRegistro) {
    if (posicaoAtual == -1) {
        NoBinario novoNo = {novoRegistro, -1, -1};
        fseek(arquivoArvore, 0, SEEK_END);
        int posicao = ftell(arquivoArvore) / sizeof(NoBinario);
        fwrite(&novoNo, sizeof(NoBinario), 1, arquivoArvore);
        return posicao;
    }

    NoBinario noAtual;
    fseek(arquivoArvore, posicaoAtual * sizeof(NoBinario), SEEK_SET);
    fread(&noAtual, sizeof(NoBinario), 1, arquivoArvore);

    if (novoRegistro.chave < noAtual.registro.chave) {
        if (noAtual.esquerda == -1) {
            noAtual.esquerda = inserirArvore(arquivoArvore, noAtual.esquerda, novoRegistro);
        } else {
            noAtual.esquerda = inserirArvore(arquivoArvore, noAtual.esquerda, novoRegistro);
        }
    } else {
        if (noAtual.direita == -1) {
            noAtual.direita = inserirArvore(arquivoArvore, noAtual.direita, novoRegistro);
        } else {
            noAtual.direita = inserirArvore(arquivoArvore, noAtual.direita, novoRegistro);
        }
    }

    fseek(arquivoArvore, posicaoAtual * sizeof(NoBinario), SEEK_SET);
    fwrite(&noAtual, sizeof(NoBinario), 1, arquivoArvore);

    return posicaoAtual;
}

void criarArvore(FILE* arquivoEntrada, const char* nomeArquivoArvore) {

    FILE* arquivoArvore = fopen(nomeArquivoArvore, "wb+");
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da arvore binaria");
        exit(1);
    }

    Registro registro;
    int raiz = -1;

    while (fread(&registro, sizeof(Registro), 1, arquivoEntrada) == 1) {
        raiz = inserirArvore(arquivoArvore, raiz, registro);
    }

    fclose(arquivoEntrada);
    fclose(arquivoArvore);

    printf("Arvore binaria criada com sucesso no arquivo: %s\n", nomeArquivoArvore);
}

int buscarArvore( const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug) {

    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da arvore binaria");
        exit(1);
    }

    NoBinario noAtual;
    int posicao = 0;

    while (1) {
        fseek(arquivoArvore, posicao * sizeof(NoBinario), SEEK_SET);
        fread(&noAtual, sizeof(NoBinario), 1, arquivoArvore);
        if (estatisticas) estatisticas->transferencias++;

        if (debug) {
            printf("Visitando nó na posição %d: chave=%d\n", posicao, noAtual.registro.chave);
        }

        if (noAtual.registro.chave == registro->chave) {
            *registro = noAtual.registro;
            if (debug) {
                printf("Chave %d encontrada na posição %d.\n", registro->chave, posicao);
            }
            return 1;
        }

        if (estatisticas) estatisticas->comparacoes++;
        if (registro->chave < noAtual.registro.chave) {
            if (noAtual.esquerda == -1) {
                break; 
            }
            posicao = noAtual.esquerda; 
        } else {
            if (noAtual.direita == -1) {
                break; 
            }
            posicao = noAtual.direita; 
        }
    }

    if (debug) {
        printf("Chave %d não encontrada.\n", registro->chave);
    }

    return 0;
}