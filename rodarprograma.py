import subprocess
import os

subprocess.run(['gcc', '*.c', '-o', 'teste.exe'])
subprocess.run(['teste.exe', '1', '100', '1', '1', '-P'])
os.remove('teste.exe')
