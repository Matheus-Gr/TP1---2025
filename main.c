#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexado.h"
#include "estatisticas.h"
#include "arvorebinaria.h"
#include "arvoreb.h"
#include "tipos.h"

void lerArquivoBinario(const char *caminhoArquivo) {
    FILE *arquivo = fopen(caminhoArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    int contador = 0;

    printf("Conteudo do arquivo %s:\n", caminhoArquivo);
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
        if(registro.chave % 500000 == 0){
        printf("Registro %d:\n", ++contador);
        printf("  Chave -lendo a um por um: %d\n", registro.chave);
        printf("  Dado1: %ld\n", registro.dado1);
        printf("  Dado2: %s\n", registro.dado2);
    }}

    fclose(arquivo);
}

void lerChavesArquivoBinario(const char *caminhoArquivo) {
    FILE *arquivo = fopen(caminhoArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    int contador = 0;
    long tamanhoRegistro = sizeof(Registro);

    printf("Chaves a cada 10000 registros do arquivo %s:\n", caminhoArquivo);
    while (fread(&registro, tamanhoRegistro, 1, arquivo) == 1) {
        printf("Registro %d - Chave: %d\n", contador + 1, registro.chave);
        contador += 10000;
        if (fseek(arquivo, contador * tamanhoRegistro, SEEK_SET) != 0) {
            break;
        }
    }

    fclose(arquivo);
}

void metodo4() {
    printf("Executando o Metodo 4: Árvore B*\n");
    // Placeholder para implementação futura.
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <metodo> <quantidade> <ordem> <chave> [-P]\n", argv[0]);
        printf("<metodo>: 1 - Seq. Indexado, 2 - Arvore Binaria, 3 - Arvore B, 4 - Arvore B*\n");
        printf("<quantidade>: numero de registros do arquivo\n");
        printf("<ordem>: 1 - Ascendente, 2 - Descendente, 3 - Aleatorio\n");
        printf("<chave>: chave a ser pesquisada\n");
        printf("[-P]: opcional, imprime chaves pesquisadas\n");
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int ordem = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int debug = (argc == 6 && strcmp(argv[5], "-P") == 0);

    char nomeArquivo[100];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "./arquivos/arquivo-%d-%d.bin", quantidade, ordem);

    printf("Nome do arquivo: %s\n", nomeArquivo);
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    if (debug) {
        printf("Metodo escolhido: %d\n", metodo);
        printf("Quantidade de registros: %d\n", quantidade);
        printf("Ordem do arquivo: %d\n", ordem);
        printf("Chave a ser pesquisada: %d\n", chave);
        printf("Opcao de imprimir chaves ativada.\n");
        printf("____________________________________\n");
    }

    Registro registro;
    registro.chave = chave;

    Apontador arvore;

    Estatisticas estatisticas;
    inicializarEstatisticas(&estatisticas);

    Registro *registros = (Registro *) malloc( quantidade * sizeof(Registro));
    fread(registros, sizeof(Registro), quantidade, arquivo);
    printf("Chave 1000900: %d", registros[1000900].chave);


    const char* arvorebin = "arvorebin.bin";

    int resultado = 0;

    switch (metodo) {
        case 1:
            resultado = pesquisaIndexada(
                &registro,
                quantidade,
                ordem,
                arquivo,
                &estatisticas,
                debug);
            break;
        case 2:
            break;
            criarArvore(arquivo, arvorebin);
            printf("Criado\n");
            lerArvore(arvorebin);
            resultado = buscarArvore(arvorebin,&registro,&estatisticas,debug);
        case 3:

            inicializa(&arvore);

            printf("INSERINDO NA ARVORE...\n");


            for(int i = 0; i < quantidade; i++)
                Insere(registros[i], &arvore, &estatisticas);
            
            printf("INSERIDO\n");
            fclose(arquivo);

            if (arvore == NULL) {
                printf("Arvore binaria nn foi criada corretamente!\n");
                return 0;
            }else {
                printf("Arvore binaria criada corretamente!\n");
                printf("Iniciando pesquisa...\n");
            }

            resultado = pesquisaArvoreB(&registro, arvore, &estatisticas);
            break;
        case 4:
            metodo4();
            break;
        default:
            printf("Metodo inválido! Escolha entre 1, 2, 3 ou 4.\n");
            return 1;
    }
             
            
    printf("_______________________________________________________________\n");
    if (resultado){
        printf("Nome do arquivo: %s\n", nomeArquivo);
        printf("Registro encontrado!\n");
        lerRegistro(&registro);
        finalizarEstatisticas(&estatisticas);
    } else {
        printf("Registro nao encontrado.\n");
    }
    

    return 0;
}
