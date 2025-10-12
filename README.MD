# 📚 Projetos de Estrutura de Dados em C

Bem-vindo ao meu repositório de projetos acadêmicos para a disciplina de Estrutura de Dados, do curso de Engenharia de Software. Este espaço contém uma coleção de aplicações em C desenvolvidas para aplicar e solidificar o conhecimento em algoritmos e estruturas de dados fundamentais.

---

## 🚀 Projetos Desenvolvidos

Aqui estão os projetos contidos neste repositório, mostrando uma progressão de conceitos.

### ⚔️ 1. War Game (Clone de Console)
Um clone simplificado do jogo de tabuleiro War (Risk), desenvolvido para o console. O objetivo é aplicar conceitos fundamentais de gerenciamento de memória e estruturas em um contexto de jogo, com um mapa e exércitos dinâmicos.

* **Conceitos Aplicados:**
    * `Structs` para representar os territórios.
    * `Alocação Dinâmica de Memória` (`calloc`, `free`) para criar um mapa de tamanho variável.
    * `Ponteiros` para manipulação de dados.
    * Lógica de jogo interativa com aleatoriedade (`rand()`).


### 🕵️‍♂️ 2. Detective Quest
Um jogo interativo de exploração em console onde o jogador navega por uma mansão (representada por uma `Árvore Binária`), coleta pistas (armazenadas em uma `Árvore Binária de Busca`) e utiliza uma `Tabela Hash` para cruzar informações e resolver um mistério.

* **Conceitos Aplicados:**
    * `Árvore Binária` para a criação de mapas.
    * `Árvore Binária de Busca (BST)` para armazenamento e listagem ordenada de dados.
    * `Tabela Hash` (com Encadeamento Separado) para mapeamento chave-valor.
    * `Pilha` para implementar a funcionalidade de "voltar" (backtracking).



### ♟️ 3. Jogo de Análise de Algoritmos
Uma ferramenta de console para gerenciar um inventário de componentes, permitindo ao usuário aplicar e comparar a performance de diferentes algoritmos de ordenação e busca.

* **Conceitos Aplicados:**
    * `Vetores` (Arrays) de `Structs`.
    * **Algoritmos de Ordenação (O(n²)):** `Bubble Sort`, `Insertion Sort`, `Selection Sort`.
    * **Algoritmos de Busca:** `Busca Sequencial` e `Busca Binária`.
    * **Análise de Algoritmos:** Contagem de comparações e medição de tempo de execução com `<time.h>`.


### 🧱 4. Tetris Stack
Um simulador de gerenciamento de peças de Tetris que utiliza as estruturas de Fila e Pilha para controlar o fluxo de peças do jogo.

* **Conceitos Aplicados:**
    * `Fila Circular` para gerenciar as peças que estão por vir.
    * `Pilha` para a mecânica de "reservar" uma peça.
    * Integração entre Fila (FIFO) e Pilha (LIFO) para criar a lógica do jogo.


---

## 🛠️ Como Compilar e Executar

Todos os projetos foram desenvolvidos em C e podem ser compilados usando o GCC (MinGW no Windows).

#### 1. Compilação

Abra seu terminal, navegue até a pasta de um dos projetos (ex: `cd DetectiveQuest`) e execute o comando de compilação. Vamos usar `jogo.exe` como nome padrão para o executável:

```bash
gcc main.c -o jogo.exe