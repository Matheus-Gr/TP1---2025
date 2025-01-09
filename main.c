#include <stdio.h>
#include <stdlib.h>

void metodo1() {
    printf("Executando o Método 1: Acesso Sequencial Indexado\n");
    // Aqui será implementada a lógica completa do método 1 futuramente.
}

void metodo2() {
    printf("Executando o Método 2: Árvore Binária de Pesquisa para Memória Externa\n");
    // Placeholder para implementação futura.
}

void metodo3() {
    printf("Executando o Método 3: Árvore B\n");
    // Placeholder para implementação futura.
}

void metodo4() {
    printf("Executando o Método 4: Árvore B*\n");
    // Placeholder para implementação futura.
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <método> <quantidade> <situação> <chave> [-P]\n", argv[0]);
        printf("<método>: 1 - Seq. Indexado, 2 - Árvore Binária, 3 - Árvore B, 4 - Árvore B*\n");
        printf("<quantidade>: número de registros do arquivo\n");
        printf("<situação>: 1 - Ascendente, 2 - Descendente, 3 - Aleatório\n");
        printf("<chave>: chave a ser pesquisada\n");
        printf("[-P]: opcional, imprime chaves pesquisadas\n");
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int imprimirChaves = (argc == 6 && strcmp(argv[5], "-P") == 0);

    printf("Método escolhido: %d\n", metodo);
    printf("Quantidade de registros: %d\n", quantidade);
    printf("Situação do arquivo: %d\n", situacao);
    printf("Chave a ser pesquisada: %d\n", chave);
    if (imprimirChaves) {
        printf("Opção de imprimir chaves ativada.\n");
    }

    switch (metodo) {
        case 1:
            metodo1();
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
            printf("Método inválido! Escolha entre 1, 2, 3 ou 4.\n");
            return 1;
    }

    return 0;
}
