#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arquivos.h"
#include "../tipos.h"

void lerIndicesDoArquivo(const char *nomeArquivo, int *indices, int quantidadeRegistros) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de índices");
        exit(1);
    }

    for (int i = 0; i < quantidadeRegistros; i++) {
        if (fread(&indices[i], sizeof(int), 1, arquivo) != 1) {
            printf("Erro ao ler indice %d do arquivo\n", i);
            perror("Erro ao ler indice do arquivo");
            exit(1);
        }

        // printf("Indice %d: %d\n", i, indices[i]);
    }

    fclose(arquivo);
}

void gerarArquivoBinario(const char *caminhoArquivo, int quantidadeRegistros, int situacao, const char *arquivoIndices) {
    FILE *arquivo = fopen(caminhoArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    srand(time(NULL));

    int *chavesGeradas = (int *)calloc(quantidadeRegistros, sizeof(int));
    int *indicesAleatorios = NULL;

    if (situacao == 3) {
        indicesAleatorios = (int *)malloc(quantidadeRegistros * sizeof(int));
        lerIndicesDoArquivo(arquivoIndices, indicesAleatorios, quantidadeRegistros);
    }

    size_t elementosGravados = 0;

    for (int i = 0; i < quantidadeRegistros; i++) {
        int chave;

        if (situacao == 1) {
            chave = i; 
        } else if (situacao == 2) {
            chave = quantidadeRegistros - i - 1;
        } else if (situacao == 3) {
            chave = indicesAleatorios[i]; 
        } else {
            fprintf(stderr, "Situação inválida!\n");
            exit(1);
        }

        registro.chave = chave;
        registro.dado1 = chave + 13;
        snprintf(registro.dado2, sizeof(registro.dado2), "Registro %d", chave);

        size_t gravado = fwrite(&registro, sizeof(Registro), 1, arquivo);
        elementosGravados += gravado;
    }

    printf("Registros gravados: %zu\n", elementosGravados);
    
    fclose(arquivo);
    free(chavesGeradas);
    free(indicesAleatorios);  // Liberar memória usada pelos índices aleatórios
    printf("Arquivo %s gerado com sucesso!\n", caminhoArquivo);
}

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
        printf("Registro %d:\n", ++contador);
        printf("  Chave: %d\n", registro.chave);
        printf("  Dado1: %ld\n", registro.dado1);
        printf("  Dado2: %s\n", registro.dado2);
    }

    fclose(arquivo);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <comando> <nome_arquivo> [quantidade] [situacao]\n", argv[0]);
        printf("Comando: gerar ou ler\n");
        printf("Situacao: 1 - Ordenado ascendente, 2 - Ordenado descendente, 3 - Aleatório\n");
        return 1;
    }

    const char *comando = argv[1];
    const char *nomeArquivo = argv[2];

    char caminhoArquivo[100];
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "%s.bin", nomeArquivo);

    //./indices-aleatorios-{tamanho}.txt
    

    if (strcmp(comando, "gerar") == 0) {
        if (argc != 5) {
            printf("Uso para gerar: %s gerar <nome_arquivo> <quantidade> <situacao>\n", argv[0]);
            return 1;
        }
        int quantidade = atoi(argv[3]);
        int situacao = atoi(argv[4]);

        char caminhoIndices[100];
        snprintf(caminhoIndices, sizeof(caminhoIndices), "./indices-aleatorios-%d.bin", quantidade);


        if (situacao < 1 || situacao > 3) {
            printf("Situacao invalida! Use 1, 2 ou 3.\n");
            return 1;
        }

        gerarArquivoBinario(caminhoArquivo, quantidade, situacao, caminhoIndices);
    } else if (strcmp(comando, "ler") == 0) {
        lerArquivoBinario(caminhoArquivo);
    } else {
        printf("Comando invalido! Use 'gerar' ou 'ler'.\n");
        return 1;
    }

    return 0;
}
