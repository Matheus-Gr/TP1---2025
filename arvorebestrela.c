#include "arvorebestrela.h"  // Inclui definições da Árvore B*

/*------------------------------------*/
/* Função: inicializa_b_estrela       */
/* Descrição: Inicializa a raiz como  */
/*            NULL.                   */
/* Parâmetros:                        */
/*   Arvore - Ponteiro para a raiz.   */
/*------------------------------------*/
void inicializa_b_estrela(ApontadorEstrela *Arvore) {
    *Arvore = NULL;  // Árvore inicia vazia
}

/*-----------------------------------------------------------*/
/* Função: pesquisaBEstrela                                  */
/* Descrição: Busca um registro, diferenciando páginas       */
/*            internas (chaves) e externas (registros).      */
/* Retorno: 1 se encontrado, 0 caso contrário.               */
/* Parâmetros:                                               */
/*   x - Registro com a chave buscada.                       */
/*   Ap - Página atual na recursão.                          */
/*   estatisticas - Contador de operações.                   */
/*   debug - Ativa logs.                                     */
/*-----------------------------------------------------------*/
int pesquisaBEstrela(Registro *x, ApontadorEstrela Ap, Estatisticas *estatisticas, int debug) {
    int i;
    ApontadorEstrela Pag = Ap;

    if (Ap == NULL) return 0;  // Árvore vazia

    if (Pag->Pt == Interna) {  // Página interna (não folha)
        i = 1;
        estatisticas->comparacoes++;
        // Busca binária na página interna
        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]) {
            i++;
            estatisticas->comparacoes++;
        }
        estatisticas->comparacoes++;
        // Decide qual filho seguir
        if (x->chave < Pag->UU.U0.ri[i - 1])
            return pesquisaBEstrela(x, Pag->UU.U0.pi[i - 1], estatisticas, debug);
        else
            return pesquisaBEstrela(x, Pag->UU.U0.pi[i], estatisticas, debug);
    } else {  // Página externa (folha)
        i = 1;
        estatisticas->comparacoes++;
        // Busca binária na folha
        while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave) {
            i++;
            estatisticas->comparacoes++;
        }
        estatisticas->comparacoes++;
        if (x->chave == Pag->UU.U1.re[i - 1].chave) {  // Chave encontrada
            *x = Pag->UU.U1.re[i - 1];
            return 1;
        } else {
            return 0;
        }
    }
}

/*-----------------------------------------------------------*/
/* Função: InsereNaPaginaExterna                             */
/* Descrição: Insere registro em folha não cheia.            */
/* Parâmetros:                                               */
/*   Ap - Página externa.                                    */
/*   Reg - Registro a inserir.                               */
/*   estatisticas - Contador de operações.                   */
/*-----------------------------------------------------------*/
void InsereNaPaginaExterna(ApontadorEstrela Ap, Registro Reg, Estatisticas *estatisticas) {
    int NaoAchouPosicao;
    int k = Ap->UU.U1.ne;  // Número atual de registros
    NaoAchouPosicao = (k > 0);

    // Encontra posição de inserção (ordem decrescente)
    while (NaoAchouPosicao) {
        estatisticas->comparacoesPP++;
        if (Reg.chave > Ap->UU.U1.re[k - 1].chave) {
            NaoAchouPosicao = 0;
            break;
        }
        // Desloca registros para a direita
        Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
        k--;
        if (k < 1) NaoAchouPosicao = 0;
    }
    // Insere o registro e atualiza contagem
    Ap->UU.U1.re[k] = Reg;
    Ap->UU.U1.ne++;
}

/*-----------------------------------------------------------*/
/* Função: InsereNaPaginaInterna                             */
/* Descrição: Insere chave em página interna não cheia.      */
/* Parâmetros:                                               */
/*   Ap - Página interna.                                    */
/*   Reg - Chave a inserir.                                  */
/*   ApDir - Ponteiro para o filho direito.                  */
/*   estatisticas - Contador de operações.                   */
/*-----------------------------------------------------------*/
void InsereNaPaginaInterna(ApontadorEstrela Ap, TipoChave Reg, ApontadorEstrela ApDir, Estatisticas *estatisticas) {
    int NaoAchouPosicao;
    int k = Ap->UU.U0.ni;  // Número atual de chaves
    NaoAchouPosicao = (k > 0);

    // Encontra posição de inserção
    while (NaoAchouPosicao) {
        estatisticas->comparacoesPP++;
        if (Reg >= Ap->UU.U0.ri[k - 1]) {
            NaoAchouPosicao = 0;
            break;
        }
        // Desloca chaves e ponteiros para a direita
        Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--;
        if (k < 1) NaoAchouPosicao = 0;
    }
    // Insere a chave e atualiza contagem
    Ap->UU.U0.ri[k] = Reg;
    Ap->UU.U0.pi[k + 1] = ApDir;
    Ap->UU.U0.ni++;
}

/*-----------------------------------------------------------*/
/* Função: Ins_b_estrela                                     */
/* Descrição: Lógica recursiva de inserção com splits        */
/*            otimizados para B*.                            */
/* Parâmetros:                                               */
/*   Reg - Registro a inserir.                               */
/*   Ap - Página atual.                                      */
/*   cresceu - Flag de crescimento.                          */
/*   RegRetorno - Chave promovida após split.                */
/*   ApRetorno - Nova página após split.                     */
/*   estatisticas - Contador de operações.                   */
/*-----------------------------------------------------------*/
void Ins_b_estrela(Registro Reg, ApontadorEstrela Ap, short *cresceu, TipoChave *RegRetorno, ApontadorEstrela *ApRetorno, Estatisticas *estatisticas) {
    long i = 1;
    long j;
    ApontadorEstrela ApTemp;

    estatisticas->comparacoesPP++;
    if (Ap->Pt == Externa) {  // Inserção em folha
        *cresceu = 1;
        *RegRetorno = Reg.chave;
        *ApRetorno = NULL;

        // Encontra posição na folha
        while (i < Ap->UU.U1.ne && Reg.chave > Ap->UU.U1.re[i - 1].chave) {
            estatisticas->comparacoesPP++;
            i++;
        }

        estatisticas->comparacoesPP++;
        if (Reg.chave == Ap->UU.U1.re[i - 1].chave) {  // Chave duplicada
            *cresceu = 0;
            return;
        }

        if (Reg.chave < Ap->UU.U1.re[i - 1].chave) i--;

        // Se a folha não está cheia, insere diretamente
        if (Ap->UU.U1.ne < MMB2) {
            InsereNaPaginaExterna(Ap, Reg, estatisticas);
            *cresceu = 0;
            return;
        }

        // Split da folha (overflow)
        ApTemp = (ApontadorEstrela)malloc(sizeof(PaginaEstrela));
        ApTemp->UU.U1.ne = 0;
        ApTemp->Pt = Externa;

        // Divide os registros entre a página atual e a nova
        if (i < MB2 + 1) {
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[MMB2 - 1], estatisticas);
            Ap->UU.U1.ne--;
            InsereNaPaginaExterna(Ap, Reg, estatisticas);
        } else {
            InsereNaPaginaExterna(ApTemp, Reg, estatisticas);
        }

        for (j = MB2 + 1; j <= MMB2; j++)
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[j - 1], estatisticas);

        // Prepara chave e página para promoção
        *RegRetorno = Ap->UU.U1.re[MB2].chave;
        Ap->UU.U1.ne = MB2;
        *ApRetorno = ApTemp;
        return;
    } else {  // Inserção em página interna
        estatisticas->comparacoesPP++;
        while (i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i - 1]) {
            i++;
            estatisticas->comparacoesPP++;
        }

        estatisticas->comparacoesPP++;
        if (Reg.chave < Ap->UU.U0.ri[i - 1]) i--;

        // Chamada recursiva para o filho adequado
        Ins_b_estrela(Reg, Ap->UU.U0.pi[i], cresceu, RegRetorno, ApRetorno, estatisticas);

        if (!*cresceu) return;

        // Se a página interna não está cheia, insere a chave
        if (Ap->UU.U0.ni < MMB) {
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno, estatisticas);
            *cresceu = 0;
            return;
        }

        // Split da página interna (overflow)
        ApTemp = (ApontadorEstrela)malloc(sizeof(PaginaEstrela));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        // Divide as chaves e ponteiros
        if (i < MB + 1) {
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[MMB - 1], Ap->UU.U0.pi[MMB], estatisticas);
            Ap->UU.U0.ni--;
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno, estatisticas);
        } else {
            InsereNaPaginaInterna(ApTemp, *RegRetorno, *ApRetorno, estatisticas);
        }

        for (j = MB + 2; j <= MMB; j++)
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j], estatisticas);

        // Atualiza contagem e ponteiros
        Ap->UU.U0.ni = MB;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[MB + 1];
        *RegRetorno = Ap->UU.U0.ri[MB];
        *ApRetorno = ApTemp;
    }
}

/*-----------------------------------------------------------*/
/* Função: Insere_b_estrela                                  */
/* Descrição: Função principal de inserção na Árvore B*.     */
/*            Cria a raiz se a árvore estiver vazia.         */
/* Parâmetros:                                               */
/*   Reg - Registro a inserir.                               */
/*   Ap - Ponteiro para a raiz.                              */
/*   estatisticas - Contador de operações.                   */
/*   debug - Ativa logs.                                     */
/*-----------------------------------------------------------*/
void Insere_b_estrela(Registro Reg, ApontadorEstrela *Ap, Estatisticas *estatisticas, int debug) {
    if (*Ap == NULL) {  // Árvore vazia: cria uma folha
        PaginaEstrela *ApTemp = (PaginaEstrela *)malloc(sizeof(PaginaEstrela));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = Reg;
        *Ap = ApTemp;
        return;
    }

    // Log de depuração para chaves específicas
    if (Reg.chave % 100000 == 0 && debug)
        printf("Inserindo ->%d\n", Reg.chave);

    short Cresceu;
    TipoChave RegRetorno;
    PaginaEstrela *ApRetorno = NULL, *ApTemp = NULL;

    // Chama a função recursiva de inserção
    Ins_b_estrela(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, estatisticas);

    estatisticas->comparacoesPP++;
    if (Cresceu) {  // Split propagou até a raiz
        ApTemp = (PaginaEstrela *)malloc(sizeof(PaginaEstrela));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;
        *Ap = ApTemp;  // Atualiza a raiz
    }
}

/*-----------------------------------------------------------*/
/* Função: liberaArvoreBEstrela                              */
/* Descrição: Libera memória alocada para a árvore B*.       */
/* Parâmetros:                                               */
/*   Arvore - Raiz da árvore.                                */
/*-----------------------------------------------------------*/
void liberaArvoreBEstrela(ApontadorEstrela Arvore) {
    if (Arvore == NULL) return;

    if (Arvore->Pt == Externa) {  // Folha: libera diretamente
        free(Arvore);
    } else {  // Página interna: libera filhos recursivamente
        for (int i = 0; i <= Arvore->UU.U0.ni; i++) {
            liberaArvoreBEstrela(Arvore->UU.U0.pi[i]);
        }
        free(Arvore);
    }
}

/*-----------------------------------------------------------*/
/* Função: printArvoreBEstrela                               */
/* Descrição: Exibe a estrutura da árvore B* de forma        */
/*            hierárquica (uso em depuração).                */
/* Parâmetros:                                               */
/*   Arvore - Raiz da árvore.                                */
/*   nivel - Profundidade atual (para indentação).           */
/*-----------------------------------------------------------*/
void printArvoreBEstrela(ApontadorEstrela Arvore, int nivel) {
    if (Arvore == NULL) return;

    // Indentação para representar níveis
    for (int i = 0; i < nivel; i++)
        printf("    ");

    if (Arvore->Pt == Interna) {  // Página interna
        printf("[INTERNA Nivel %d] Chaves: ", nivel);
        for (int i = 0; i < Arvore->UU.U0.ni; i++) {
            printf("%d ", Arvore->UU.U0.ri[i]);
        }
        printf("\n");

        // Chama recursivamente para os filhos
        for (int i = 0; i <= Arvore->UU.U0.ni; i++) {
            printArvoreBEstrela(Arvore->UU.U0.pi[i], nivel + 1);
        }
    } else {  // Página externa
        printf("[EXTERNA Nivel %d] Registros: ", nivel);
        for (int i = 0; i < Arvore->UU.U1.ne; i++) {
            printf("%d ", Arvore->UU.U1.re[i].chave);
        }
        printf("\n");
    }
}