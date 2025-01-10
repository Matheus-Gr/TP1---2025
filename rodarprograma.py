import subprocess
import os

comando_base = 'teste.exe'
metodo = str(1)
quantidade = str(2000)
ordem = str(1)
chave = str(1678)
imprimir = '-P'

subprocess.run(['gcc', '*.c', '-o', comando_base])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
