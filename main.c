#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"          // Definições de Registro, Estatisticas, etc.
#include "indexado.h"       // Funções de pesquisa indexada
#include "estatisticas.h"   // Funções de estatísticas
#include "arvorebinaria.h"  // Funções de árvore binária
#include "arvorebestrela.h" // Funções de árvore B*
#include "arvoreb.h"        // Funções de árvore B

double prepross;

/**
 * @brief Lê e imprime todos os registros de um arquivo binário.
 *
 * Exibe os campos de cada registro na saída padrão.
 *
 * @param caminhoArquivo Caminho do arquivo binário a ser lido.
 */
void lerArquivoBinario(const char *caminhoArquivo)
{
    FILE *arquivo = fopen(caminhoArquivo, "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    Registro registro;
    int contador = 0;

    printf("Conteudo do arquivo %s:\n", caminhoArquivo);
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1)
    {
        printf("Registro %d:\n", ++contador);
        lerRegistro(&registro); // Usa a função lerRegistro de tipos.c
    }

    fclose(arquivo);
}

/**
 * @brief Função principal do programa.
 *
 * Executa experimentos de pesquisa em arquivos binários utilizando diferentes métodos:
 * 1 - Pesquisa Sequencial Indexada
 * 2 - Árvore Binária
 * 3 - Árvore B
 * 4 - Árvore B*
 *
 * Parâmetros esperados na linha de comando:
 * <metodo> <quantidade> <ordem> <chave> [-P]
 *
 * - <metodo>: 1 (Indexada), 2 (Árvore Binária), 3 (Árvore B), 4 (Árvore B*)
 * - <quantidade>: número de registros do arquivo
 * - <ordem>: 1 (Ascendente), 2 (Descendente), 3 (Aleatório)
 * - <chave>: chave a ser pesquisada
 * - [-P]: opcional, imprime chaves pesquisadas e informações extras
 *
 * @param argc Número de argumentos.
 * @param argv Vetor de argumentos.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int main(int argc, char *argv[])
{
    if (argc < 5)
    {
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

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    if (debug)
    {
        printf("Nome do arquivo: %s\n", nomeArquivo);
        printf("Metodo escolhido: %d\n", metodo);
        printf("Quantidade de registros: %d\n", quantidade);
        printf("Ordem dos registros: %d\n", ordem);
        printf("Chave a ser pesquisada: %d\n", chave);
        printf("Opcao de imprimir chaves ativada.\n");
    }

    Registro registro;
    registro.chave = chave;

    Apontador arvore;
    ApontadorEstrela arvoreEstrela;

    Estatisticas estatisticas;

    Registro *registros = NULL;

    const char *arvorebin = "arvorebin.bin";

    int resultado = 0;
    int profundidade = 0;

    inicializarEstatisticas(&estatisticas);

    switch (metodo)
    {
    case 1:
        // Pesquisa Sequencial Indexada
        resultado = pesquisaIndexada(
            &registro,
            quantidade,
            ordem,
            arquivo,
            &estatisticas,
            debug);
        finalizarEstatisticasPequisa(&estatisticas);
        break;
    case 2:
        // Árvore Binária em arquivo
        if (debug)
            printf("1) Alocando array de registros\n");
        registros = (Registro *)malloc(quantidade * sizeof(Registro));
        estatisticas.transferenciasPP++;
        fread(registros, sizeof(Registro), quantidade, arquivo);

        if (debug)
        {
            printf("Ultima chave do arquivo: %d\n", registros[quantidade - 1].chave);
            printf("2) Criando arvore binaria\n");
        }

        criarArvore(registros, quantidade, arvorebin, &estatisticas, debug);
        finalizarPreProcessamento(&estatisticas);

        if (debug)
        {
            lerArvore(arvorebin);
            printf("3) Pre-Processamento encerrado\n");
        }
        if (debug)
            printf("4) Iniciando pesquisa\n");
        inicializarTimerPesquisa(&estatisticas);
        resultado = buscarArvore(arvorebin, &registro, &estatisticas, debug);
        finalizarEstatisticasPequisa(&estatisticas);
        break;
    case 3:
        // Árvore B (em memória)
        if (debug)
            printf("1) Inicializando arvore\n");
        inicializa(&arvore);

        if (debug)
            printf("2) Alocando array de registros\n");
        registros = (Registro *)malloc(quantidade * sizeof(Registro));
        estatisticas.transferenciasPP++;
        fread(registros, sizeof(Registro), quantidade, arquivo);

        if (debug)
        {
            printf("Ultima chave do arquivo: %d\n", registros[quantidade - 1].chave);
            printf("3) Inserindo registros na arvore\n");
        }

        for (int i = 0; i < quantidade; i++)
            Insere(registros[i], &arvore, &estatisticas, debug);

        fclose(arquivo);
        free(registros);

        finalizarPreProcessamento(&estatisticas);

        if (arvore == NULL)
        {
            printf("Arvore binaria nn foi criada corretamente!\n");
            return 0;
        }
        else
        {
            printf("Arvore binaria criada corretamente!\n");
            printf("Iniciando pesquisa...\n");
        }

        if (debug)
            printf("4) Pre-Processamento encerrado\n");
        profundidade = calculaProfundidade(arvore);
        printf("Profundidade da Arvore B: %d\n", profundidade);
        if (debug)
            printf("5) Iniciando pesquisa\n");
        inicializarTimerPesquisa(&estatisticas);
        resultado = pesquisaArvoreB(&registro, arvore, &estatisticas);
        finalizarEstatisticasPequisa(&estatisticas);
        break;
    case 4:
        // Árvore B* (em memória)
        if (debug)
            printf("1) Inicializando arvore B*\n");
        inicializa_b_estrela(&arvoreEstrela);

        if (debug)
            printf("2) Alocando array de registros\n");
        registros = (Registro *)malloc(quantidade * sizeof(Registro));
        estatisticas.transferenciasPP++;
        fread(registros, sizeof(Registro), quantidade, arquivo);

        if (debug)
        {
            printf("Ultima chave do arquivo: %d\n", registros[quantidade - 1].chave);
            printf("3) Inserindo registros na arvore\n");
        }

        for (int i = 0; i < quantidade; i++)
            Insere_b_estrela(registros[i], &arvoreEstrela, &estatisticas, debug);

        fclose(arquivo);
        free(registros);

        finalizarPreProcessamento(&estatisticas);

        if (debug)
            printf("4) Pre-Processamento encerrado\n");
        profundidade = calculaProfundidadeBEstrela(arvoreEstrela);
        printf("Profundidade da Arvore B*: %d\n", profundidade);

        if (debug)
            printf("5) Iniciando pesquisa\n");
        inicializarTimerPesquisa(&estatisticas);
        resultado = pesquisaBEstrela(&registro, arvoreEstrela, &estatisticas, debug);
        finalizarEstatisticasPequisa(&estatisticas);
        break;
    default:
        printf("Metodo inválido! Escolha entre 1, 2, 3 ou 4.\n");
        return 1;
    }

    printf("_______________________________________________________________\n");
    if (resultado)
    {
        printf("Registro encontrado!\n");
        lerRegistro(&registro); // Usa a função lerRegistro de tipos.c
        printarEstatisticas(&estatisticas);
        salvarEstatisticas(metodo, quantidade, ordem, chave, &estatisticas, "estatisticas.txt");
    }
    else
    {
        printf("Registro nao encontrado.\n");
    }

    fclose(arquivo);
    return 0;
}