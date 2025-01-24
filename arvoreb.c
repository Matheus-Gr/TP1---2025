#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

void inicializa(Apontador *arvore){
    *arvore = NULL;
}


void InsereNaPagina (Apontador apontador, Registro Reg, Apontador ApDir, Estatisticas *estatisticas){
    int NaoAchouPosicao;
    int k = apontador->nFilhos; 
    NaoAchouPosicao = (k > 0);
    
    //Procura em qual posicao o item devera ser inserido na pagina
    while(NaoAchouPosicao){
        // estatisticas->preProcessamento.comparacoes += 1;
        if(Reg.chave >= apontador->registros[k-1].chave){
            NaoAchouPosicao = 0;
            break;
        }
        apontador->registros[k] = apontador->registros[k-1];
        apontador->ponteiros[k+1] = apontador->ponteiros[k];
        k--;
        if(k < 1)
            NaoAchouPosicao = 0;
    }

    apontador->registros[k] = Reg;
    apontador->ponteiros[k+1] = ApDir;
    apontador->nFilhos++;
}

void Ins(Registro registro,
         Apontador apontador,
         short *cresceu,
         Registro *regRetorno, 
         Apontador *apRetorno,
         Estatisticas *estatisticas){
        
    long i = 1; //Onde o item deve ser inserido
    long j;
    
    Apontador apTemp;
    
    //Verifica se a arvore esta vazia ou se chegou no nodo folha
    if(apontador == NULL){
        *cresceu = 1;
        (*regRetorno) = registro;
        (*apRetorno) = NULL;

        return;
    }

    //Realiza uma pesquisa na pagina para saber se ele existe nela
    // estatisticas->preProcessamento.comparacoes += 1;
    while(i < apontador->nFilhos &&
          registro.chave > apontador->registros[i-1].chave){
        i++;
        //  estatisticas->preProcessamento.comparacoes += 1;
    }

    // estatisticas->preProcessamento.comparacoes += 1;
    if(registro.chave == apontador->registros[i-1].chave){
        //!Erro: Registro ja esta presente
        *cresceu = 0;
        return;
    }

    //Verifica se iremos para a sub arvore a esquerda (true) ou direita (0)
    // estatisticas->preProcessamento.comparacoes += 1;
    if(registro.chave < apontador->registros[i-1].chave) i--;
    

    Ins(registro, apontador->ponteiros[i], cresceu, regRetorno, apRetorno, estatisticas);

    //Quando passar por este if, significa que chegou no nodo folha, entao podemos inserir
    //Tambem ira passar quando o no filho deu overflow e a arvore ira crescer, pois um item subiu
    if(!*cresceu) return;
        
    //Verifica se a pagina NAO ira crescer, mesmo apos a recursao e adiciona o item no nodo
    if(apontador->nFilhos < MM){ 
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
        *cresceu = 0;

        return;
    }

    //Overflow: Pagina tem que ser dividida
    //Criando uma nova pagina
    apTemp = (Apontador) malloc(sizeof(Pagina));
    apTemp->nFilhos = 0;
    apTemp->ponteiros[0] = NULL;

    //Verifica para onde a chave ira
    if(i < M + 1){
        //Insere o item na pagina que ja existe
        //Coloca o ultimo registro na nova pagina
        InsereNaPagina(apTemp, apontador->registros[MM - 1], apontador->ponteiros[MM], estatisticas);
        apontador->nFilhos--;
        //Insere o novo item na pagina atual
        InsereNaPagina(apontador, *regRetorno, *apRetorno, estatisticas);
    }
    
    // Inserindo o item que deu o overflow na pagina vizinha
    else InsereNaPagina(apTemp, *regRetorno, *apRetorno, estatisticas);
        
    //Colocando os valores excedentes e colocando na pagina nova
    for(j = M + 2; j <= MM; j++)
        InsereNaPagina(apTemp, apontador->registros[j-1], apontador->ponteiros[j], estatisticas);
        
    apontador->nFilhos = M;
    apTemp->ponteiros[0] = apontador->ponteiros[M + 1];
    *regRetorno = apontador->registros[M];
    *apRetorno = apTemp;
}



void Insere(Registro registro,
            Apontador *folha,
            Estatisticas *estatisticas){
    short cresceu;
    Registro regRetorno;
    Pagina *apRetorno, *apTemp;

    Ins(registro, *folha, &cresceu, &regRetorno, &apRetorno, estatisticas);
    
    //Verifica se a raiz da arvore vai crescer a raiz
    if(cresceu){
        apTemp = (Pagina*) malloc(sizeof(Pagina));
        apTemp->nFilhos = 1;
        apTemp->registros[0] = regRetorno;
        apTemp->ponteiros[1] = apRetorno;
        apTemp->ponteiros[0] = *folha; 
        *folha = apTemp;
    }
}


int pesquisaArvoreB(Registro *registro,
                    Apontador apontador,
                    Estatisticas *estatisticas){
                        
    if(apontador == NULL) return 0;

    long i = 1;
    estatisticas->comparacoes += 1;
    while(i < apontador->nFilhos && registro->chave > apontador->registros[i-1].chave){
        i++;
        estatisticas->comparacoes += 1;
    }

    estatisticas->comparacoes += 1;
    if(registro->chave == apontador->registros[i-1].chave){
        *registro = apontador->registros[i-1];
        estatisticas->comparacoes += 1;
        return 1;
    }

    estatisticas->comparacoes += 1;
    if(registro->chave < apontador->registros[i-1].chave) {
        return pesquisaArvoreB(registro, apontador->ponteiros[i-1], estatisticas);
    }else {
        return pesquisaArvoreB(registro, apontador->ponteiros[i], estatisticas);
    } 

    
}