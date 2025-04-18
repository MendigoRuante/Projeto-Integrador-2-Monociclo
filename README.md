# Projeto Monociclo MIPS

Simulador de um processador MIPS monociclo implementado em C. O projeto foi desenvolvido por uma equipe de 3 integrantes ao longo de 4 sprints.

## 😎 Descrição

Este simulador é capaz de interpretar e executar instruções binárias no formato MIPS, utilizando um ciclo único de clock para cada instrução (arquitetura monociclo).

Durante a execução, o simulador:
- Carrega um arquivo `.mem` contendo as instruções binárias.
- Permite salvar:
  - Arquivo `.asm` com as instruções já decodificadas.
  - Arquivo `.dat` com o conteúdo da memória de dados.

O sistema também possui uma funcionalidade de **retrocesso (back)**, utilizando uma **pilha de estados**, possibilitando retornar múltiplas etapas anteriores da simulação.

## 🛠️ Tecnologias Utilizadas

- Linguagem C
- Arquitetura MIPS Monociclo
- Estrutura de dados (pilha)

## 🚀 Como Executar

1. Compile o código em C com o compilador de sua preferência:
   ```bash
   gcc main.c Monociclo.c -o Monociclo
2. Execute o programa
   ```bash
   ./Monociclo
