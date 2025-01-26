import subprocess
import os


comando_base = 'teste.exe'
metodo = str(1)
quantidade = str(2000000)
ordem = str(1)
chave = str(400000)
imprimir = '-P'

# gcc -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -m64 *.c -o  pragapeste.exe -Wall

subprocess.run(['gcc', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE',
               '-D_LARGEFILE64_SOURCE', '-m64', '*.c', '-o', comando_base, '-Wall'])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
if metodo == 2:
    os.remove("arvorebin.bin")
