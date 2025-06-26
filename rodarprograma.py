#  @file rodarprograma.py
#  @brief Script para compilar e executar testes de algoritmos de pesquisa.

import subprocess
import os
import random
import pandas as pd
import dataframe_image as dfi
import plotly.express as px
import matplotlib.pyplot as plt
import numpy as np

# gcc -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -m64 *.c -o pragapeste.exe -Wall

##
# @brief Compila todos os arquivos .c do diretorio atual em um executavel.
# @param comando_base Nome do executavel de saida.


def compilar(comando_base):
    subprocess.run(['gcc', '-m64', '*.c', '-o', comando_base, '-Wall'])

##
# @brief Executa o programa compilado com os parametros fornecidos.
# @param comando_base Caminho do executavel.
# @param metodo Metodo de pesquisa (1, 2, 3, 4).
# @param quantidade Quantidade de registros.
# @param ordem Ordem dos registros.
# @param chave Chave a ser pesquisada.
# @param imprimir Se True, ativa o modo debug (-P).
# @param tempo_limite Tempo maximo de execucao em segundos.
# @return True se houve timeout, False caso contrario.


def rodar_programa(comando_base, metodo, quantidade, ordem, chave, imprimir=False, tempo_limite=60):
    if imprimir:
        debug = '-P'
    else:
        debug = ''
    try:
        resultado = subprocess.run(
            [comando_base, metodo, quantidade, ordem, chave, debug],
            capture_output=False,
            text=True,
            timeout=tempo_limite,
        )
    except subprocess.TimeoutExpired:
        return True

    if resultado.returncode != 0:
        print(f'O comando {comando_base} falhou com o seguinte erro:')
        print(resultado.stderr)
        return False
    return False

##
# @brief Gera uma lista de chaves aleatorias e diferentes entre si.
# @param chave_maxima Valor maximo (exclusivo) para as chaves.
# @param quantidade_chaves Quantidade de chaves a gerar.
# @return Lista de chaves unicas.


def gerar_chaves_diferentes(chave_maxima, quantidade_chaves=10):
    chaves = []
    while len(chaves) < quantidade_chaves:
        chave = random.randint(0, chave_maxima - 1)
        if chave not in chaves:
            chaves.append(chave)
    return chaves

##
# @brief Le o arquivo de estatisticas e gera uma tabela de medias agrupadas por metodo, quantidade e ordem.
#        Salva a tabela como imagem PNG usando dataframe_image.
# @param metodo Metodo de pesquisa a ser filtrado.
# @param arquivo Caminho do arquivo de estatisticas.


def gerar_tabela_resultados(metodo, arquivo='estatisticas.txt'):
    if not os.path.exists(arquivo):
        print(f'Arquivo {arquivo} nao encontrado.')
        return

    root_folder = 'graficos/'

    with open(arquivo, 'r') as f:
        linhas = f.readlines()
    dados = []
    for linha in linhas:
        partes = linha.strip().split('-')
        if len(partes) == 10:
            m, quantidade, ordem, chave, transferencias_pp, comparacoes_pp, tempo_execucao_pp, transferencias, comparacoes, tempo_execucao = partes
            if int(m) == metodo:
                dados.append({
                    'Metodo': int(m),
                    'Quantidade': int(quantidade),
                    'Ordem': int(ordem),
                    'TransferenciasPP': float(transferencias_pp),
                    'ComparacoesPP': float(comparacoes_pp),
                    'TempoExecucaoPP': float(tempo_execucao_pp),
                    'Transferencias': float(transferencias),
                    'Comparacoes': float(comparacoes),
                    'TempoExecucao': float(tempo_execucao),
                })
    df = pd.DataFrame(dados)

    contagens = df.groupby(['Metodo', 'Quantidade', 'Ordem']
                           ).size().reset_index(name='Execucoes')
    medias = df.groupby(['Metodo', 'Quantidade', 'Ordem'],
                        as_index=False).mean()

    medias = pd.merge(medias, contagens, on=['Metodo', 'Quantidade', 'Ordem'])

    dfi.export(medias, f'{root_folder}tabela_{metodo}.png', max_rows=-1)
    print(
        f'Tabela do metodo {metodo} gerada: tabela_{metodo}.png')
    return

##
# @brief Executa testes automatizados para todos os metodos, quantidades, ordens e chaves.
#        Para cada combinacao, executa o programa e salva os resultados.
# @param comando_base Caminho do executavel.
# @param metodos Metodos de pesquisa a testar.
# @param quantidades Quantidades de registros.
# @param ordens Ordens dos registros.
# @param quantidade_chaves Quantidade de chaves a testar por combinacao.


def rodar_testes(comando_base,
                 metodos=[1, 2, 3, 4],
                 quantidades=[100, 1000, 10000, 100000, 1000000],
                 ordens=[1, 2, 3],
                 quantidade_chaves=10):

    for metodo in metodos:
        for ordem in ordens:
            if metodo == 1 and ordem == 3:
                print(
                    f'Metodo {metodo} com ordem {ordem} nao e suportado. Pulando teste.')
                continue
            for quantidade in quantidades:
                timeout = False

                chaves = gerar_chaves_diferentes(quantidade, quantidade_chaves)

                print('---------=-=-=-=-=-=-=-=-=-=-=-=----------')
                print(f'Chaves geradas: {chaves}')
                print('---------=-=-=-=-=-=-=-=-=-=-=-=---------')

                for i, chave in enumerate(chaves):
                    print(
                        f'Rodando teste n{i + 1}: Metodo={metodo}, Quantidade={quantidade}, Ordem={ordem}, Chave={chave}')
                    timeout = rodar_programa(comando_base, str(metodo), str(
                        quantidade), str(ordem), str(chave), tempo_limite=(60*5))
                    if timeout:
                        print(
                            'Tempo limite excedido. Interrompendo execucao. Numero de execucoes deste teste: ', i)
                        break
                if timeout:
                    break
        gerar_tabela_resultados(metodo)

##
# @brief Remove arquivos de saida, executavel e imagens geradas.
# @param comando_base Caminho do executavel.
# @param arquivos_saida Nome do arquivo de estatisticas.


def limpar_arquivos(comando_base, arquivos_saida='estatisticas.txt'):
    if os.path.exists(arquivos_saida):
        os.remove(arquivos_saida)
    if os.path.exists(comando_base):
        os.remove(comando_base)
    # remover arquivos de tabela e graficos
    for arquivo in os.listdir('.'):
        if arquivo.startswith('tabela_') or arquivo.startswith('grafico_'):
            os.remove(arquivo)

    print(f'Arquivos {arquivos_saida} e {comando_base} removidos.')

##
# @brief Gera graficos de barras (comparacoes, transferencias, tempo) a partir do arquivo de estatisticas.
#        Salva os graficos em PNG na pasta 'graficos/'.
# @param arquivo Caminho do arquivo de estatisticas.


def gerar_graficos(arquivo='estatisticas.txt'):
    if not os.path.exists(arquivo):
        print(f'Arquivo {arquivo} nao encontrado.')
        return

    root_folder = 'graficos/'

    # Carrega os dados
    df = pd.read_csv(arquivo, sep='-', header=None)
    df.columns = ['Metodo', 'Quantidade', 'Ordem', 'Chave',
                  'Transferencias', 'Comparacoes', 'TempoExecucao',
                  'TransferenciasPP', 'ComparacoesPP', 'TempoExecucaoPP']

    # Calcula medias agrupadas por metodo, quantidade e ordem
    medias = df.groupby(['Metodo', 'Quantidade', 'Ordem'], as_index=False)[
        ['Transferencias', 'Comparacoes', 'TempoExecucao']].mean()

    metodos = sorted(medias['Metodo'].unique())

    for metodo in metodos:
        df_metodo = medias[medias['Metodo'] == metodo]

        # Grafico de Comparacoes em barras
        plt.figure(figsize=(10, 6))
        for ordem in sorted(df_metodo['Ordem'].unique()):
            df_ordem = df_metodo[df_metodo['Ordem'] == ordem]
            plt.bar([f"{int(q)}\n(Ordem {ordem})" for q in df_ordem['Quantidade']],
                    df_ordem['Comparacoes'],
                    label=f'Ordem {ordem}')
        plt.title(f'Metodo {metodo}: Comparacoes por Registros')
        plt.xlabel('Registros')
        plt.ylabel('Comparacoes')
        plt.legend()
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.savefig(f'{root_folder}grafico_comparacoes_metodo_{metodo}.png')
        plt.close()

        # Grafico de Transferencias em barras
        plt.figure(figsize=(10, 6))
        for ordem in sorted(df_metodo['Ordem'].unique()):
            df_ordem = df_metodo[df_metodo['Ordem'] == ordem]
            plt.bar([f"{int(q)}\n(Ordem {ordem})" for q in df_ordem['Quantidade']],
                    df_ordem['Transferencias'],
                    label=f'Ordem {ordem}')
        plt.title(f'Metodo {metodo}: Transferencias por Registros')
        plt.xlabel('Registros')
        plt.ylabel('Transferencias')
        plt.legend()
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.savefig(f'{root_folder}grafico_transferencias_metodo_{metodo}.png')
        plt.close()

        # Grafico de Tempo de Execucao agrupado por ordem
        plt.figure(figsize=(10, 6))
        ordens = sorted(df_metodo['Ordem'].unique())
        quantidades = sorted(df_metodo['Quantidade'].unique())
        largura = 0.2
        x = np.arange(len(quantidades))

        for i, ordem in enumerate(ordens):
            tempos = []
            for q in quantidades:
                valor = df_metodo[(df_metodo['Ordem'] == ordem)
                                  & (df_metodo['Quantidade'] == q)]
                tempos.append(valor['TempoExecucao'].values[0]
                              if not valor.empty else 0)
            plt.bar(x + i * largura, tempos,
                    width=largura, label=f'Ordem {ordem}')

        plt.xticks(x + largura, quantidades)
        plt.title(f'Metodo {metodo}: Tempo de Execucao por Registros')
        plt.xlabel('Registros')
        plt.ylabel('Tempo de Execucao (ms)')
        plt.legend()
        plt.tight_layout()
        plt.savefig(f'{root_folder}grafico_tempo_metodo_{metodo}.png')
        plt.close()

    print('Graficos de barras gerados com sucesso.')


##
# @brief Executa a sequencia completa:
#        - Limpa arquivos antigos
#        - Compila o programa
#        - Executa todos os testes
#        - Gera graficos dos resultados
if __name__ == '__main__':
    comando_base = 'teste.exe'
    limpar_arquivos(comando_base)
    compilar(comando_base)
    rodar_testes(comando_base)
    gerar_graficos()
    # rodar_programa(comando_base, '1', '1000', '1', '55', True)
