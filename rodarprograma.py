import subprocess
import os


comando_base = 'teste.exe'
metodo = str(2)
quantidade = str(200000)
ordem = str(3)
chave = str(int(quantidade) - 10)
imprimir = '-P'

# gcc -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -m64 *.c -o  pragapeste.exe -Wall

subprocess.run(['gcc', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE',
               '-D_LARGEFILE64_SOURCE', '-m64', '*.c', '-o', comando_base, '-Wall'])
subprocess.run([comando_base, metodo, quantidade, ordem, chave, imprimir])
os.remove(comando_base)
# if metodo == 2:
#     os.remove("arvorebin.bin")
