//instrucoes: add sub and or addi lw sw (beq j)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Limite_de_Instrucacoes 256 // Aloca o espaço na memória para criar o vetor de instruções
#define Tam_Instrucao 17 // 16 bits + 1 para o \0

typedef struct {
    char binario[Tam_Instrucao]; // Armazena a instrução como string binária
} instrucao;

void Ler_Arquivo();
int ula();

int main() {
    printf("...");
    int op_menuprincipal=1;
    while(op_menuprincipal != 0){
        printf("\n\n|=====================Menu Principal=====================|\n|0. Sair                                                 |\n|1. Carregar memória de instruções (.mem)                |\n|2. Carregar memória de Dados (.dat)                     |\n|3. Imprimir memórias (instruções e dados)               |\n|4. Imprimir banco de registradores                      |\n|5. Imprimir todo o simulador (registradores e memórias) |\n|6. Salvar .asm                                          |\n|7. Salvar .dat                                          |\n|8. Executa Programa (run)                               |\n|9. Executa uma instrução (Step)                         |\n|10. Volta uma instrução (Back)                          |\n|========================================================|\nEscolha uma opção:\n");
        scanf("%d", &op_menuprincipal);
        switch(op_menuprincipal){
            default:
                printf("Opção Inválida...\n");
            break;
            case 0:
                printf("Saindo...");
            break;
            case 1:
                Ler_Arquivo();
            break;
        }
    }
    
    return 0;
}

void Ler_Arquivo(){
    // Pergunta ao usuário o arquivo a ser carregado
    char nome_arquivo[100];
    printf("Digite o nome do arquivo a ser aberto\n");
    scanf("%99s", nome_arquivo);

    // Abre o arquivo
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    instrucao Instrucoes[Limite_de_Instrucacoes];
    int cont = 0;

    // O loop vai coletar as instruções enquanto houver algo nas linhas 
    while (fgets(Instrucoes[cont].binario, sizeof(Instrucoes[cont].binario), file)) {
        // Exclui o \n
        Instrucoes[cont].binario[strcspn(Instrucoes[cont].binario, "\n")] = 0;
        
        // Evita armazenar linhas vazias
        if (strlen(Instrucoes[cont].binario) > 0) {
            cont++;
        }
    }

    fclose(file);

    // Printa as instruções 
    printf("Instruções armazenadas:\n");
    for (int i = 0; i < cont; i++) {
        printf("%d: %s\n", i + 1, Instrucoes[i].binario);
    }
}

int ula(){
    return 0;
}