# Projeto de Pesquisa em Arquivos Binários com Estruturas de Dados

Este projeto, desenvolvido para a disciplina de Estruturas de Dados II, tem como objetivo comparar o desempenho de diferentes métodos de pesquisa em arquivos binários de registros. O sistema permite a geração, manipulação e busca eficiente de dados utilizando diversas estruturas clássicas de indexação e árvores.

<!-- ---

## Sumário

- [Objetivo](#objetivo)
- [Como Utilizar](#como-utilizar)
  - [Manual](#manualmente)
  - [Scripts Automáticos](#usar-scripts-automáticos)
- [Funcionalidades](#funcionalidades)
- [Estrutura do Projeto](#estrutura-do-projeto)
  - [Diretórios e Arquivos](#diretórios-e-arquivos)
  - [Scripts Python](#scripts-python)

--- -->

## Objetivo

- Implementar e comparar métodos de pesquisa em arquivos binários, incluindo:
  - Pesquisa Sequencial Indexada
  - Árvore Binária (em arquivo)
  - Árvore B (em memória)
  - Árvore B\* (em memória)
- Coletar estatísticas detalhadas de desempenho: número de comparações, transferências e tempo de execução.

---

## Como Utilizar

### Manualmente

1. **Compilar gerador de arquivos**

   Compile o script `gerar_arquivos.c` para gerar os arquivos binários de teste:

   ```bash
   cd arquivos
   gcc *.c -o gerar_arquivos.exe -Wall
   ```

2. **Gerar arquivos de teste**

   Execute o gerador para criar os arquivos binários:

   ```bash
   gerar_arquivos.exe gerar <quantidade> <ordem>
   ```

   - `<quantidade>`: número de registros a serem gerados
   - `<ordem>`: `0` (ordenado), `1` (reverso), `2` (aleatório)

   Para ler arquivos já existentes:

   ```bash
   gerar_arquivos.exe ler <nome_arquivo_sem_extensao>
   ```

3. **Compilar o projeto**

   ```bash
   gcc -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -m64 *.c -o app.exe -Wall
   ```

4. **Executar o programa**

   Rode o executável informando o método, quantidade, ordem e chave:

   ```bash
   app.exe <metodo> <quantidade> <ordem> <chave> [-P]
   ```

---

### Usar Scripts Automáticos

1. **Gerar arquivos de teste**

   ```bash
   cd arquivos
   python gerartodos.py
   ```

2. **Executar os experimentos**

   ```bash
   python rodarprograma.py
   ```

---

<!--
## Funcionalidades

- Geração de arquivos binários com registros em diferentes ordens (ordenado, reverso, aleatório).
- Construção e manipulação de estruturas de dados (índices, árvores binárias, árvores B e B\*).
- Busca por chave com coleta de estatísticas detalhadas (comparações, transferências, tempo).
- Scripts Python para automação da geração de arquivos, execução dos testes e análise dos resultados com gráficos e tabelas.

--- -->

## Estrutura do Projeto

### Diretórios e Arquivos

- `main.c` — Função principal, responsável por orquestrar a execução e interação com o usuário.
- `tipos.h/.c` — Definição dos tipos de dados e estruturas utilizadas no projeto.
- `arvoreb.h/.c`, `arvorebestrela.h/.c`, `arvorebinaria.h/.c`, `indexado.h/.c` — Implementações das estruturas de dados para pesquisa e indexação.
- `gerar_arquivos.c` — Geração de arquivos binários de teste.
- `estatisticas.h/.c` — Funções para coleta e impressão de estatísticas.
- `arquivos/` — Diretório contendo arquivos binários gerados e scripts relacionados.
- `graficos/` — Diretório com gráficos gerados a partir dos resultados.

### Scripts Python

- `gerartodos.py` — Gera arquivos de teste automaticamente.
- `rodarprograma.py` — Compila, executa e analisa os resultados dos experimentos.

---

Se precisar de ajuda para configurar o ambiente, exemplos de comandos, ou documentação adicional, é só avisar!
