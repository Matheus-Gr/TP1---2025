#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexado.h"
#include "estatisticas.h"


void metodo1() {
    printf("Executando o Metodo 1: Acesso Sequencial Indexado\n");
}

void metodo2() {
    printf("Executando o Metodo 2: Árvore Binária de Pesquisa para Memória Externa\n");
    // Placeholder para implementação futura.
}

void metodo3() {
    printf("Executando o Metodo 3: Árvore B\n");
    // Placeholder para implementação futura.
}

void metodo4() {
    printf("Executando o Metodo 4: Árvore B*\n");
    // Placeholder para implementação futura.
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <metodo> <quantidade> <situacao> <chave> [-P]\n", argv[0]);
        printf("<metodo>: 1 - Seq. Indexado, 2 - Arvore Binaria, 3 - Arvore B, 4 - Arvore B*\n");
        printf("<quantidade>: numero de registros do arquivo\n");
        printf("<situacao>: 1 - Ascendente, 2 - Descendente, 3 - Aleatorio\n");
        printf("<chave>: chave a ser pesquisada\n");
        printf("[-P]: opcional, imprime chaves pesquisadas\n");
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int debug = (argc == 6 && strcmp(argv[5], "-P") == 0);

    char nomeArquivo[100];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "./arquivos/arquivo-%d-%d.bin", quantidade, situacao);

    

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    if (debug) {
        printf("Nome do arquivo: %s\n", nomeArquivo);
        printf("Metodo escolhido: %d\n", metodo);
        printf("Quantidade de registros: %d\n", quantidade);
        printf("Situacao do arquivo: %d\n", situacao);
        printf("Chave a ser pesquisada: %d\n", chave);
        printf("Opcao de imprimir chaves ativada.\n");
        printf("____________________________________\n");
    }

    Registro registro;
    registro.chave = chave;

    Estatisticas estatisticas;
    inicializarEstatisticas(&estatisticas);

    int resultado = 0;

    switch (metodo) {
        case 1:
            resultado = pesquisaIndexada(&registro,quantidade,arquivo,&estatisticas, debug);
            
            if (resultado) {
                printf("Registro encontrado!\n");
            } else {
                printf("Registro nao encontrado.\n");
            }

            // metodo1();
            break;
        case 2:
            metodo2();
            break;
        case 3:
            metodo3();
            break;
        case 4:
            metodo4();
            break;
        default:
            printf("Metodo inválido! Escolha entre 1, 2, 3 ou 4.\n");
            return 1;
    }

    if (resultado) lerRegistro(&registro);
    

    return 0;
}
