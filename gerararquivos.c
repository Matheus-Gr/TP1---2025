#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int chave;
    long dado1;
    char dado2[5000];
} Registro;

void gerarArquivoBinario(const char *nomeArquivo, int quantidadeRegistros, int situacao) {
    char caminhoArquivo[100];
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "./arquivos/%s", nomeArquivo);

    FILE *arquivo = fopen(caminhoArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    srand(time(NULL));

    for (int i = 0; i < quantidadeRegistros; i++) {
        registro.chave = (situacao == 1) ? i : 
                         (situacao == 2) ? quantidadeRegistros - i - 1 : 
                         rand() % (quantidadeRegistros * 10);

        registro.dado1 = rand() % 100000;
        snprintf(registro.dado2, sizeof(registro.dado2), "Registro %d", i);

        fwrite(&registro, sizeof(Registro), 1, arquivo);
    }

    fclose(arquivo);
    printf("Arquivo %s gerado com sucesso!\n", nomeArquivo);
}

void lerArquivoBinario(const char *nomeArquivo) {
    char caminhoArquivo[100];
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "./arquivos/%s", nomeArquivo);

    FILE *arquivo = fopen(caminhoArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    int contador = 0;

    printf("Conteudo do arquivo %s:\n", nomeArquivo);
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

    if (strcmp(comando, "gerar") == 0) {
        if (argc != 5) {
            printf("Uso para gerar: %s gerar <nome_arquivo> <quantidade> <situacao>\n", argv[0]);
            return 1;
        }
        int quantidade = atoi(argv[3]);
        int situacao = atoi(argv[4]);

        if (situacao < 1 || situacao > 3) {
            printf("Situacao invalida! Use 1, 2 ou 3.\n");
            return 1;
        }

        gerarArquivoBinario(nomeArquivo, quantidade, situacao);
    } else if (strcmp(comando, "ler") == 0) {
        lerArquivoBinario(nomeArquivo);
    } else {
        printf("Comando invalido! Use 'gerar' ou 'ler'.\n");
        return 1;
    }

    return 0;
}
