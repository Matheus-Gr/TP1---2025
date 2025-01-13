import subprocess
import os

comando_base = 'teste.exe'
metodo = str(2)
quantidade = str(200)
ordem = str(3)
chave = str(67)
imprimir = '-P'

subprocess.run(['gcc', '*.c', '-o', comando_base])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
