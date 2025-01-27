#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

void inicializa(Apontador *arvore){
    *arvore = NULL;
}

void InsereNaPagina(Apontador apontador, Registro Reg, Apontador ApDir, Estatisticas *estatisticas) {
    if (apontador == NULL) return;
    
    int k = apontador->nFilhos;
    int NaoAchouPosicao = (k > 0);
    
    while (NaoAchouPosicao) {
        if (Reg.chave >= apontador->registros[k-1].chave) {
            NaoAchouPosicao = 0;
            break;
        }
        apontador->registros[k] = apontador->registros[k-1];
        apontador->ponteiros[k+1] = apontador->ponteiros[k];
        k--;
        if (k < 1)
            NaoAchouPosicao = 0;
    }
    apontador->registros[k] = Reg;
    apontador->ponteiros[k+1] = ApDir;
    apontador->nFilhos++;
}

void Ins(Registro registro, Apontador apontador, short *cresceu, Registro *regRetorno, Apontador *apRetorno, Estatisticas *estatisticas, int debug) {
    estatisticas->profundidadeAB++;
    if(debug && (estatisticas->profundidadeAB % 50000 == 0))
        printf("Profundidade: %d\n", estatisticas->profundidadeAB);

    //i informa onde será inserido o registro
    long i = 0;
    Apontador apTemp;

    if (apontador == NULL) {
        *cresceu = 1;
        *regRetorno = registro;
        *apRetorno = NULL;
        estatisticas->profundidadeAB--;
        return;
    }

    while (i < apontador->nFilhos && registro.chave > apontador->registros[i].chave) {
        estatisticas->comparacoesPP++;
        i++;
    }

    if (i < apontador->nFilhos && registro.chave == apontador->registros[i].chave) {
        estatisticas->comparacoesPP++;
        *cresceu = 0;
        estatisticas->profundidadeAB--;
        return;
    }

    Ins(registro, apontador->ponteiros[i], cresceu, regRetorno, apRetorno, estatisticas, debug);

    if (!*cresceu){
        estatisticas->profundidadeAB--;
        return;
    }

    if (apontador->nFilhos < MM) {
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
        *cresceu = 0;
        estatisticas->profundidadeAB--;
        return;
    }

    // ** Correção: Fazer o split no meio corretamente **
    apTemp = (Apontador) malloc(sizeof(Pagina));
    if (apTemp == NULL) {
        printf("Erro: Falha na alocacao de memoria em 'Ins'!\n");
        exit(1);
    }

    apTemp->nFilhos = 0;
    apTemp->ponteiros[0] = NULL;

    int meio = MM / 2;
    if (i <= meio) {
        InsereNaPagina(apTemp, apontador->registros[MM - 1], apontador->ponteiros[MM], estatisticas);
        apontador->nFilhos--;
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
    } else {
        InsereNaPagina(apTemp, *regRetorno, *apRetorno, estatisticas);
    }

    for (int j = meio + 1; j < MM; j++) {
        InsereNaPagina(apTemp, apontador->registros[j], apontador->ponteiros[j + 1], estatisticas);
    }

    apontador->nFilhos = meio;
    apTemp->ponteiros[0] = apontador->ponteiros[meio + 1];

    *regRetorno = apontador->registros[meio];
    *apRetorno = apTemp;
    estatisticas->profundidadeAB--;
}

void Insere(Registro registro, Apontador *folha, Estatisticas *estatisticas, int debug) {
    short cresceu;
    Registro regRetorno;
    Pagina *apRetorno, *apTemp;

    if (registro.chave % 100000 == 0 && debug) printf("Inserindo ->%d\n", registro.chave);

    Ins(registro, *folha, &cresceu, &regRetorno, &apRetorno, estatisticas, debug);
    
    estatisticas->comparacoesPP++;
    if (cresceu) {
        apTemp = (Apontador) malloc(sizeof(Pagina));  
        if (apTemp == NULL) {  
            printf("Erro: Falha na alocacao de memoria em 'Insere'!\n");  
            exit(1);  
        }  
        apTemp->nFilhos = 1;  
        apTemp->registros[0] = regRetorno;  
        apTemp->ponteiros[1] = apRetorno;  
        apTemp->ponteiros[0] = *folha;   
        *folha = apTemp;  
    }  

}

int pesquisaArvoreB(Registro *registro, Apontador apontador, Estatisticas *estatisticas) {
    if (apontador == NULL) {
        printf("Erro: Arvore vazia!\n");
        exit(1);
    }

    long i = 1;
    estatisticas->comparacoes++;
    while (i < apontador->nFilhos && registro->chave > apontador->registros[i-1].chave) {
        i++;
        estatisticas->comparacoes++;
    }

    estatisticas->comparacoes++;
    if (registro->chave == apontador->registros[i-1].chave) {
        *registro = apontador->registros[i-1];
        estatisticas->comparacoes++;
        
        return 1;
    }

    estatisticas->comparacoes++;
    if (registro->chave < apontador->registros[i-1].chave) {
        return pesquisaArvoreB(registro, apontador->ponteiros[i-1], estatisticas);
    } else {
        return pesquisaArvoreB(registro, apontador->ponteiros[i], estatisticas);
    } 
}
