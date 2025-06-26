#  @file gerartodos.py
#  @brief Gera arquivos binarios de registros com diferentes quantidades e situacoes.

import subprocess
import os
import random
import struct

quantidades = [100, 1000, 10000, 100000, 1000000]
situacoes = [1, 2, 3]
comando_base = 'arquivos.exe'


##
# @brief Gera uma lista de indices embaralhados de 0 ate tamanho-1.
# @param tamanho Quantidade de indices a serem gerados.
# @return Lista de indices embaralhados.
def gerar_indices_embaralhados(tamanho):
    indices = list(range(tamanho))
    random.shuffle(indices)
    return indices


##
# @brief Salva uma lista de indices inteiros em um arquivo binario.
# @param tamanho Quantidade de indices.
# @param indices Lista de indices a serem salvos.
def salvar_indices(tamanho, indices):
    nome_arquivo = f'./indices-aleatorios-{tamanho}.bin'
    with open(nome_arquivo, 'wb') as f:
        for indice in indices:
            f.write(struct.pack('i', indice))


# Gera e salva arquivos de indices embaralhados para cada quantidade
for tamanho in quantidades:
    indices_embaralhados = gerar_indices_embaralhados(tamanho)
    salvar_indices(tamanho, indices_embaralhados)

# Compila o programa de geracao de arquivos binarios
subprocess.run(['gcc', '*.c', '-o', comando_base])


##
# @brief Gera um arquivo binario de registros usando o programa compilado.
# @param quantidade Quantidade de registros.
# @param situacao Situacao dos registros (1: ordenado, 2: reverso, 3: aleatorio).
def gerar_arquivos(quantidade, situacao):
    nome_arquivo = f"arquivo-{quantidade}-{situacao}"
    subprocess.run([comando_base, "gerar", nome_arquivo,
                   str(quantidade), str(situacao)])


# Gera todos os arquivos binarios para cada combinacao de quantidade e situacao
for quantidade in quantidades:
    for situacao in situacoes:
        gerar_arquivos(quantidade, situacao)

# Remove o executavel apos a geracao dos arquivos
os.remove(comando_base)
