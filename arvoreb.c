#include <stdio.h>
#include <stdlib.h>
#include "tipos.h" // Inclui definições de Registro, Pagina, Estatisticas, etc.

void inicializa(Apontador *arvore)
{
    *arvore = NULL; // Árvore inicia vazia
}

void InsereNaPagina(Apontador apontador, Registro Reg, Apontador ApDir, Estatisticas *estatisticas)
{
    if (apontador == NULL)
        return; // Verificação de segurança

    int k = apontador->nFilhos;    // Número atual de filhos
    int NaoAchouPosicao = (k > 0); // Flag para busca da posição correta

    // Encontra a posição de inserção (ordem decrescente)
    while (NaoAchouPosicao)
    {
        if (Reg.chave >= apontador->registros[k - 1].chave)
        {
            NaoAchouPosicao = 0; // Posição encontrada
            break;
        }
        // Desloca registros e ponteiros para a direita
        apontador->registros[k] = apontador->registros[k - 1];
        apontador->ponteiros[k + 1] = apontador->ponteiros[k];
        k--;
        if (k < 1)
            NaoAchouPosicao = 0; // Limite mínimo
    }
    // Insere o registro e atualiza a contagem
    apontador->registros[k] = Reg;
    apontador->ponteiros[k + 1] = ApDir;
    apontador->nFilhos++;
}

void Ins(Registro registro, Apontador apontador, short *cresceu, Registro *regRetorno, Apontador *apRetorno, Estatisticas *estatisticas, int debug)
{
    long i = 0;
    Apontador apTemp;

    // Caso base: chegou a uma folha NULL (inserção requer nova página)
    if (apontador == NULL)
    {
        *cresceu = 1;
        *regRetorno = registro;
        *apRetorno = NULL;
        return;
    }

    // Encontra a posição de inserção na página atual
    while (i < apontador->nFilhos && registro.chave > apontador->registros[i].chave)
    {
        estatisticas->comparacoesPP++; // Atualiza estatísticas
        i++;
    }

    // Verifica se a chave já existe (evita duplicatas)
    if (i < apontador->nFilhos && registro.chave == apontador->registros[i].chave)
    {
        estatisticas->comparacoesPP++;
        *cresceu = 0; // Não cresce a árvore
        return;
    }

    // Chamada recursiva para o filho adequado
    Ins(registro, apontador->ponteiros[i], cresceu, regRetorno, apRetorno, estatisticas, debug);

    if (!*cresceu)
        return; // Não houve split no nível inferior

    // Se a página não está cheia, insere diretamente
    if (apontador->nFilhos < MM)
    {
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
        *cresceu = 0;
        return;
    }

    // Split da página (overflow)
    apTemp = (Apontador)malloc(sizeof(Pagina));
    if (apTemp == NULL)
    { // Verifica alocação
        printf("Erro: Falha na alocação de memória em 'Ins'!\n");
        exit(1);
    }
    apTemp->nFilhos = 0;
    apTemp->ponteiros[0] = NULL;

    int meio = MM / 2; // Ponto de divisão

    // Decide se o novo registro vai para a página esquerda ou direita
    if (i <= meio)
    {
        // Move o último registro para a nova página e insere o novo registro na atual
        InsereNaPagina(apTemp, apontador->registros[MM - 1], apontador->ponteiros[MM], estatisticas);
        apontador->nFilhos--;
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
    }
    else
    {
        InsereNaPagina(apTemp, *regRetorno, *apRetorno, estatisticas);
    }

    // Move registros da página cheia para a nova página
    for (int j = meio + 1; j < MM; j++)
    {
        InsereNaPagina(apTemp, apontador->registros[j], apontador->ponteiros[j + 1], estatisticas);
    }

    // Atualiza contagem de filhos e ponteiros
    apontador->nFilhos = meio;
    apTemp->ponteiros[0] = apontador->ponteiros[meio + 1];

    // Prepara o registro e a página para promoção
    *regRetorno = apontador->registros[meio];
    *apRetorno = apTemp;
}

void Insere(Registro registro, Apontador *folha, Estatisticas *estatisticas, int debug)
{
    short cresceu;
    Registro regRetorno;
    Pagina *apRetorno, *apTemp;

    // Log de depuração para chaves específicas
    if (registro.chave % 100000 == 0 && debug)
        printf("Inserindo ->%d\n", registro.chave);

    // Chama a função auxiliar de inserção
    Ins(registro, *folha, &cresceu, &regRetorno, &apRetorno, estatisticas, debug);

    estatisticas->comparacoesPP++;
    if (cresceu)
    { // Se a raiz precisou ser dividida
        apTemp = (Apontador)malloc(sizeof(Pagina));
        if (apTemp == NULL)
        {
            printf("Erro: Falha na alocação de memória em 'Insere'!\n");
            exit(1);
        }
        // Cria nova raiz com o registro promovido
        apTemp->nFilhos = 1;
        apTemp->registros[0] = regRetorno;
        apTemp->ponteiros[1] = apRetorno;
        apTemp->ponteiros[0] = *folha;
        *folha = apTemp; // Atualiza a raiz
    }
}

int pesquisaArvoreB(Registro *registro, Apontador apontador, Estatisticas *estatisticas)
{
    if (apontador == NULL)
    { // Árvore vazia
        printf("Erro: Arvore vazia!\n");
        exit(1);
    }

    long i = 1;
    estatisticas->comparacoes++;
    // Encontra a posição onde a chave deveria estar
    while (i < apontador->nFilhos && registro->chave > apontador->registros[i - 1].chave)
    {
        i++;
        estatisticas->comparacoes++;
    }

    estatisticas->comparacoes++;
    if (registro->chave == apontador->registros[i - 1].chave)
    { // Chave encontrada
        *registro = apontador->registros[i - 1];
        estatisticas->comparacoes++;
        return 1;
    }

    estatisticas->comparacoes++;
    // Decide se busca no filho esquerdo ou direito
    if (registro->chave < apontador->registros[i - 1].chave)
    {
        return pesquisaArvoreB(registro, apontador->ponteiros[i - 1], estatisticas);
    }
    else
    {
        return pesquisaArvoreB(registro, apontador->ponteiros[i], estatisticas);
    }
}

int calculaProfundidade(Apontador arvore)
{
    if (arvore == NULL)
        return 0; // Caso base
    // Desce pelo primeiro filho até chegar a uma folha
    return 1 + calculaProfundidade(arvore->ponteiros[0]);
}