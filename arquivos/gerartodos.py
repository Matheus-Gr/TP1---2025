import subprocess
import os
import random
import struct

quantidades = [100, 200, 2000, 20000, 200000, 2000000]
situacoes = [1, 2, 3]
comando_base = 'arquivos.exe'


def gerar_indices_embaralhados(tamanho):
    indices = list(range(tamanho))
    random.shuffle(indices)
    return indices


def salvar_indices(tamanho, indices):
    nome_arquivo = f'./indices-aleatorios-{tamanho}.bin'
    with open(nome_arquivo, 'wb') as f:
        for indice in indices:
            f.write(struct.pack('i', indice))


for tamanho in quantidades:
    indices_embaralhados = gerar_indices_embaralhados(tamanho)
    salvar_indices(tamanho, indices_embaralhados)


subprocess.run(['gcc', '*.c', '-o', comando_base])


def gerar_arquivos(quantidade, situacao):
    nome_arquivo = f"arquivo-{quantidade}-{situacao}"
    subprocess.run([comando_base, "gerar", nome_arquivo,
                   str(quantidade), str(situacao)])


for quantidade in quantidades:
    for situacao in situacoes:
        gerar_arquivos(quantidade, situacao)

os.remove(comando_base)
