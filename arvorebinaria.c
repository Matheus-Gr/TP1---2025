#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

void atualizaPonteiros(FILE* arquivoArvore, NoBinario* itemInserir)
{   
    fseek(arquivoArvore, 0, SEEK_END);
    long qtdItensArquivo = ftell(arquivoArvore) / sizeof(NoBinario);
    fseek(arquivoArvore, 0, SEEK_SET);



    if(qtdItensArquivo == 1) return;
    
    NoBinario lido;

    long ponteiro = 1;
    long desloc;
    

    do{
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        fseek(arquivoArvore, desloc, SEEK_SET);
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);

        ponteiro = (itemInserir->registro.chave > lido.registro.chave) ? lido.direita : lido.esquerda;
    }while(ponteiro != -1);

    fseek(arquivoArvore, (long) -(sizeof(NoBinario)), SEEK_CUR);
    
    if(itemInserir->registro.chave > lido.registro.chave)
        lido.direita = qtdItensArquivo;
    else 
        lido.esquerda = qtdItensArquivo;   

    fwrite(&lido, sizeof(NoBinario), 1, arquivoArvore);
    return;
}

void criarArvore(FILE* arquivoEntrada, const char* nomeArquivoArvore) {
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "wb+"); 
    if (!arquivoArvore) {
        perror("Erro ao criar o arquivo da arvore binaria");
        exit(1);
    }

    Registro lido;

    while (fread(&lido, sizeof(Registro), 1, arquivoEntrada) == 1) {

        // printf("%d\n", lido.chave);

        NoBinario no;
        no.registro = lido;
        no.esquerda = -1;
        no.direita = -1;
        fseek(arquivoArvore, 0, SEEK_END);
        if (fwrite(&no, sizeof(NoBinario), 1, arquivoArvore) != 1) {
            perror("Erro ao escrever no arquivo");
            exit(1);
        }
        atualizaPonteiros(arquivoArvore, &no);
    }

    fclose(arquivoArvore);
    printf("Arvore binaria criada com sucesso no arquivo: %s\n", nomeArquivoArvore);
}



int buscarArvore(const char* nomeArquivoArvore, Registro* registro, Estatisticas* estatisticas, int debug) {
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da árvore binária");
        exit(1);
    }

    NoBinario lido;
    long ponteiro = 1;
    long desloc;

    do{
        //Calcula o deslocamento necessario, a partir do inicio do arquivo, para chegar ao no filho do pai
        desloc = (ponteiro - 1) * sizeof(NoBinario);
        fseek(arquivoArvore, desloc, SEEK_SET);
        fread(&lido, sizeof(NoBinario), 1, arquivoArvore);
        estatisticas->transferencias += 1;

        //Caminhando o ponteiro pelo arquivo ate encontrar uma "folha" = (-1)
        if(registro->chave == lido.registro.chave){
            estatisticas->comparacoes += 1;
            *registro = lido.registro;
            return 1;
        }
        ponteiro = (registro->chave > lido.registro.chave) ? lido.direita : lido.esquerda;
        estatisticas->comparacoes += 1;

    }while(ponteiro != -1);
    
    return 0;
}

void lerArvore(const char* nomeArquivoArvore) {
    FILE* arquivoArvore = fopen(nomeArquivoArvore, "rb");
    if (!arquivoArvore) {
        perror("Erro ao abrir o arquivo da árvore binária");
        return;
    }

    NoBinario no;
    int posicao = 0;
    printf("Posicao | Chave | Esquerda | Direita\n");
    printf("----------------------------------\n");
    
    while (fread(&no, sizeof(NoBinario), 1, arquivoArvore) == 1) {
        printf("%8d | %5d | %8d | %6d\n", posicao, no.registro.chave, no.esquerda, no.direita);
        posicao++;
    }

    fclose(arquivoArvore);
}
