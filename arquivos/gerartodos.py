import subprocess

quantidades = [100, 200,  2000, 20000, 200000, 2000000]
situacoes = [1, 2, 3]
comando_base = 'arquivos.exe'

subprocess.run(['gcc', '*.c', '-o', comando_base])


def gerar_arquivos(quantidade, situacao):
    nome_arquivo = f"arquivo-{quantidade}-{situacao}"
    subprocess.run([comando_base, "gerar", nome_arquivo,
                   str(quantidade), str(situacao)])


for quantidade in quantidades:
    for situacao in situacoes:
        gerar_arquivos(quantidade, situacao)
