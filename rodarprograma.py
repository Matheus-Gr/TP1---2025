import subprocess
import os


comando_base = 'teste.exe'
metodo = str(3)
quantidade = str(2000)
ordem = str(1)
chave = str(int(quantidade) - 10)
imprimir = '-P'

# gcc -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -m64 *.c -o  pragapeste.exe -Wall

subprocess.run(['gcc', '-m64', '*.c', '-o', comando_base, '-Wall'])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
