import subprocess
import os

# Nome do executável gerado após a compilação
comando_base = 'pesquisa.exe'  # Nome do executável
metodo = str(1)                # Método 4: Árvore B*
quantidade = str(2000000)         # Quantidade de registros
ordem = str(2)                 # Ordem ascendente
chave = str(1999908)              # Chave a ser pesquisada
imprimir = '-P'                # Opção para imprimir chaves pesquisadas

# Lista de arquivos .c para compilar
arquivos_c = [
    'main.c',
    'arvorebestrela.c',        # Certifique-se de que este arquivo existe
    'indexado.c',
    'tipos.c',

    'arvorebinaria.c',         # Certifique-se de que este arquivo existe
    'estatisticas.c'           # Adicionado estatisticas.c
]

# Verifica se todos os arquivos .c existem
arquivos_faltando = [
    arquivo for arquivo in arquivos_c if not os.path.exists(arquivo)]
if arquivos_faltando:
    print("Erro: Os seguintes arquivos não foram encontrados:")
    for arquivo in arquivos_faltando:
        print(f"  - {arquivo}")
    exit(1)

try:
    # Compila o programa em C
    print("Compilando o programa em C...")
    resultado_compilacao = subprocess.run(
        ['gcc', *arquivos_c, '-o', comando_base, '-Wall'],
        capture_output=True,
        text=True
    )

    # Verifica se a compilação foi bem-sucedida
    if resultado_compilacao.returncode != 0:
        print("Erro durante a compilação:")
        print(resultado_compilacao.stderr)
        exit(1)

    print("Compilação concluída com sucesso!")

    # Executa o programa compilado
    print("Executando o programa...")
    resultado_execucao = subprocess.run(
        [comando_base, metodo, quantidade, ordem, chave, imprimir],
        capture_output=True,
        text=True
    )

    # Exibe a saída do programa
    print("Saída do programa (stdout):")
    print(resultado_execucao.stdout)

    # Exibe a saída de erro do programa
    print("Saída de erro do programa (stderr):")
    print(resultado_execucao.stderr)

    # Verifica se houve erro durante a execução
    if resultado_execucao.returncode != 0:
        print("Erro durante a execução:")
        print(resultado_execucao.stderr)

except Exception as e:
    print(f"Erro inesperado: {e}")

finally:
    # Remove o executável após a execução
    if os.path.exists(comando_base):
        print(f"Removendo o executável '{comando_base}'...")
        os.remove(comando_base)

    # Remove o arquivo "arvorebin.bin" se o método for 2
    if metodo == '2' and os.path.exists("arvorebin.bin"):
        print("Removendo o arquivo 'arvorebin.bin'...")
        os.remove("arvorebin.bin")

    print("Processo concluído.")
