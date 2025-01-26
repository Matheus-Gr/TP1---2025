#include "arvorebestrela.h"

void inicializa_b_estrela(TipoApontadorEstrela *Arvore) {
    *Arvore = NULL;
}

bool Pesquisa(TipoRegistro *x, TipoApontadorEstrela Ap, Estatisticas *estatisticas, bool debug) {
    int i;
    TipoApontadorEstrela Pag = Ap;

    if (Ap == NULL) return false;

    if (Pag->Pt == Interna) {
        i = 1;
        estatisticas->comparacoes++;
        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]) {
            i++;
            estatisticas->comparacoes++;
        }
        estatisticas->comparacoes++;
        if (x->chave < Pag->UU.U0.ri[i - 1])
            return Pesquisa(x, Pag->UU.U0.pi[i - 1], estatisticas, debug);
        else
            return Pesquisa(x, Pag->UU.U0.pi[i], estatisticas, debug);
    } else {
        i = 1;
        estatisticas->comparacoes++;
        while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave) {
            i++;
            estatisticas->comparacoes++;
        }
        estatisticas->comparacoes++;
        if (x->chave == Pag->UU.U1.re[i - 1].chave) {
            *x = Pag->UU.U1.re[i - 1];
            return true;
        } else {
            return false;
        }
    }
}

void InsereNaPaginaExterna(TipoApontadorEstrela Ap, TipoRegistro Reg, Estatisticas *estatisticas) {
    bool NaoAchouPosicao;
    int k = Ap->UU.U1.ne;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao) {
        estatisticas->comparacoes++;
        if (Reg.chave > Ap->UU.U1.re[k - 1].chave) {
            NaoAchouPosicao = false;
            break;
        }
        Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
        k--;
        if (k < 1) NaoAchouPosicao = false;
    }

    Ap->UU.U1.re[k] = Reg;
    Ap->UU.U1.ne++;
}

void InsereNaPaginaInterna(TipoApontadorEstrela Ap, TipoChave Reg, TipoApontadorEstrela ApDir, Estatisticas *estatisticas) {
    bool NaoAchouPosicao;
    int k = Ap->UU.U0.ni;
    NaoAchouPosicao = (k > 0);

    while (NaoAchouPosicao) {
        estatisticas->comparacoes++;
        if (Reg >= Ap->UU.U0.ri[k - 1]) {
            NaoAchouPosicao = false;
            break;
        }
        Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
        Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
        k--;
        if (k < 1) NaoAchouPosicao = false;
    }
    Ap->UU.U0.ri[k] = Reg;
    Ap->UU.U0.pi[k + 1] = ApDir;
    Ap->UU.U0.ni++;
}

void Ins_b_estrela(TipoRegistro Reg, TipoApontadorEstrela Ap, short *cresceu, TipoChave *RegRetorno, TipoApontadorEstrela *ApRetorno, Estatisticas *estatisticas) {
    long i = 1;
    long j;
    TipoApontadorEstrela ApTemp;

    if (Ap->Pt == Externa) {
        *cresceu = true;
        *RegRetorno = Reg.chave;
        *ApRetorno = NULL;

        estatisticas->comparacoes++;
        while (i < Ap->UU.U1.ne && Reg.chave > Ap->UU.U1.re[i - 1].chave) {
            i++;
            estatisticas->comparacoes++;
        }

        estatisticas->comparacoes++;
        if (Reg.chave == Ap->UU.U1.re[i - 1].chave) {
            *cresceu = false;
            return;
        }

        if (Reg.chave < Ap->UU.U1.re[i - 1].chave) i--;

        if (Ap->UU.U1.ne < MMB2) {
            InsereNaPaginaExterna(Ap, Reg, estatisticas);
            *cresceu = false;
            return;
        }

        ApTemp = (TipoApontadorEstrela)malloc(sizeof(TipoPaginaEstrela));
        ApTemp->UU.U1.ne = 0;
        ApTemp->Pt = Externa;

        if (i < MB2 + 1) {
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[MMB2 - 1], estatisticas);
            Ap->UU.U1.ne--;
            InsereNaPaginaExterna(Ap, Reg, estatisticas);
        } else {
            InsereNaPaginaExterna(ApTemp, Reg, estatisticas);
        }

        for (j = MB2 + 1; j <= MMB2; j++)
            InsereNaPaginaExterna(ApTemp, Ap->UU.U1.re[j - 1], estatisticas);

        *RegRetorno = Ap->UU.U1.re[MB2].chave;
        Ap->UU.U1.ne = MB2;
        *ApRetorno = ApTemp;
        return;
    } else {
        estatisticas->comparacoes++;
        while (i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i - 1]) {
            i++;
            estatisticas->comparacoes++;
        }

        estatisticas->comparacoes++;
        if (Reg.chave < Ap->UU.U0.ri[i - 1]) i--;

        Ins_b_estrela(Reg, Ap->UU.U0.pi[i], cresceu, RegRetorno, ApRetorno, estatisticas);

        if (!*cresceu) return;

        if (Ap->UU.U0.ni < MMB) {
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno, estatisticas);
            *cresceu = false;
            return;
        }

        ApTemp = (TipoApontadorEstrela)malloc(sizeof(TipoPaginaEstrela));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        if (i < MB + 1) {
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[MMB - 1], Ap->UU.U0.pi[MMB], estatisticas);
            Ap->UU.U0.ni--;
            InsereNaPaginaInterna(Ap, *RegRetorno, *ApRetorno, estatisticas);
        } else {
            InsereNaPaginaInterna(ApTemp, *RegRetorno, *ApRetorno, estatisticas);
        }

        for (j = MB + 2; j <= MMB; j++)
            InsereNaPaginaInterna(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j], estatisticas);

        Ap->UU.U0.ni = MB;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[MB + 1];
        *RegRetorno = Ap->UU.U0.ri[MB];
        *ApRetorno = ApTemp;
    }
}

void Insere_b_estrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, Estatisticas *estatisticas) {
    if (*Ap == NULL) {
        TipoPaginaEstrela *ApTemp = (TipoPaginaEstrela *)malloc(sizeof(TipoPaginaEstrela));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 1;
        ApTemp->UU.U1.re[0] = Reg;
        *Ap = ApTemp;
        return;
    }

    short Cresceu;
    TipoChave RegRetorno;
    TipoPaginaEstrela *ApRetorno = NULL, *ApTemp = NULL;

    Ins_b_estrela(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, estatisticas);

    if (Cresceu) {
        ApTemp = (TipoPaginaEstrela *)malloc(sizeof(TipoPaginaEstrela));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;
        *Ap = ApTemp;
    }
}

TipoApontadorEstrela ConstruirArvoreBStar(char *nomeArquivo, int quantidade, Estatisticas *estatisticas, bool debug) {
    clock_t inicio = clock(); // Inicia a medição

    FILE *arq = fopen(nomeArquivo, "rb");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        return NULL;
    }

    TipoRegistro *registros = (TipoRegistro *)malloc(quantidade * sizeof(TipoRegistro));
    fread(registros, quantidade, sizeof(TipoRegistro), arq);
    estatisticas->transferencias += 1;

    if (debug) {
        exibirItensArvoreBEstrela(registros, quantidade);
    }

    TipoApontadorEstrela Arvore;
    inicializa_b_estrela(&Arvore);

    for (int i = 0; i < quantidade; i++) {
        Insere_b_estrela(registros[i], &Arvore, estatisticas);
    }

    fclose(arq);
    free(registros);

    clock_t fim = clock(); // Finaliza a medição
    estatisticas->tempoPreProcessamento = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;

    return Arvore;
}

void liberaArvoreBEstrela(TipoApontadorEstrela Arvore) {
    if (Arvore == NULL) return;

    if (Arvore->Pt == Externa) {
        free(Arvore);
    } else {
        for (int i = 0; i <= Arvore->UU.U0.ni; i++) {
            liberaArvoreBEstrela(Arvore->UU.U0.pi[i]);
        }
        free(Arvore);
    }
}

void exibirItensArvoreBEstrela(TipoRegistro *registros, int quantidade) {
    for (int i = 0; i < quantidade; i++)
        printf("%d\n", registros[i].chave);
}
void printArvoreBEstrela(TipoApontadorEstrela Arvore, int nivel) {
    if (Arvore == NULL) return;

    // Imprime recuo para representar os níveis
    for (int i = 0; i < nivel; i++)
        printf("    "); // 4 espaços por nível

    if (Arvore->Pt == Interna) {
        printf("[INTERNA Nivel %d] Chaves: ", nivel);
        for (int i = 0; i < Arvore->UU.U0.ni; i++) {
            printf("%d ", Arvore->UU.U0.ri[i]);
        }
        printf("\n");

        // Recursão para filhos
        for (int i = 0; i <= Arvore->UU.U0.ni; i++) {
            printArvoreBEstrela(Arvore->UU.U0.pi[i], nivel + 1);
        }
    } else {
        printf("[EXTERNA Nivel %d] Registros: ", nivel);
        for (int i = 0; i < Arvore->UU.U1.ne; i++) {
            printf("%d ", Arvore->UU.U1.re[i].chave);
        }
        printf("\n");
    }
}