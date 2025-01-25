import subprocess
import os


comando_base = 'teste.exe'
metodo = str(3)
quantidade = str(2000000)
ordem = str(1)
chave = str(88003)
imprimir = '-P'

subprocess.run(['gcc', '*.c', '-o', comando_base, '-Wall'])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
if metodo == 2:
    os.remove("arvorebin.bin")
