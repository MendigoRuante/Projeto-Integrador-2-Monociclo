// Instrucoes: add sub and or addi lw sw (beq j)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#define Limite_de_Instrucoes 256 // Aloca o espaço na memória para criar o vetor de instruções
#define Tam_Instrucao 18 // 16 bits + 1 para o \0
#define NUM_REGISTRADORES 8 // Numero de Registradores, 3 bits = 8

typedef enum {
    TipoR,
    TipoI,
    TipoJ
} TipoInstrucao;

typedef struct {
    char binario[17];
    int op, rs, rt, rd, imm, funct, end;
    char sop[5], srs[4], srt[4], srd[4], simm[7], sfunct[4], send[13];
    TipoInstrucao tipo;
} Memoria;

//FUNÇÕES
void Ler_Arquivo();
int converteOpCode(char *line);
int binParaDec(char *line);
Memoria recebeInstrucaoR(char *line);
Memoria recebeInstrucaoI(char *line);
Memoria recebeInstrucaoJ(char *line);
void imprimeInstrucao(Memoria *instrucao);
Memoria * alocaMemoria(int tam);
int ULA(int ulaop, int operando1, int operando2);
void escrever_registrador(int reg_destino, int valor);
void imprimir_registradores();
void incrementarpc();
void modpc(int endereco);
void Verifica_Cria_MemDados(const char *arquivo);
void GravaMemDados(const char *arquivo, int endereco, int dado);
int LeMemDados(const char *arquivo, int endereco);
//==========

//GLOBAIS
Memoria* instrucoes = NULL;
int cont = 0;
int registradores[NUM_REGISTRADORES];
int pc = 0;
const char *Arquivo = "Memoria_de_dados.dat";
//==========

int main() {
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
            case 2:
                printf("Não Implementado..."); // IMPLEMENTAR
            break;
            case 3:
                if (instrucoes == NULL){
                    printf("Memória de instruções vazia...\n");
                }
                else{
                    printf("=====Memória de Instruções=====\n");
                    for(int x = 0; x < cont; x++){
                        imprimeInstrucao(instrucoes+x);
                        
                    }
                    printf("===============================\n");
                }
                //falta imprimir memoria de dados no case 3
            break;
            case 4:
                imprimir_registradores();
            break;
            case 5:
                imprimir_registradores();
                if (instrucoes == NULL){
                    printf("Memória de instruções vazia...\n");
                }
                else{
                    printf("=====Memória de Instruções=====\n");
                    for(int x = 0; x < cont; x++){
                        imprimeInstrucao(instrucoes+x);
                        
                    }
                    printf("===============================\n");
                }
                //falta imprimir instruções e dados nesse case 5
            break;
            case 98: //excluir depois, case para testes
                escrever_registrador(0, 123); //N vai fazer nada
                escrever_registrador(1, 1);
                escrever_registrador(2, 2);
                escrever_registrador(3, 3);
                escrever_registrador(4, 10);
                escrever_registrador(5, 10);
                escrever_registrador(6, 10);
                escrever_registrador(7, 10);
            break;
            case 99: //excluir depois, case para testes
                printf("PC atual: %d\n", pc);
                incrementarpc();
                printf("PC atual: %d\n", pc);
                modpc(6);
                printf("PC atual: %d\n", pc);
            break;
            case 100: //Case para teste de memória de dados/ Teste de Criar/Verificar
                Verifica_Cria_MemDados(Arquivo);
            break;
            case 101: //Testa a gravação da memória
                GravaMemDados(Arquivo, 1, 10);
                GravaMemDados(Arquivo, 10, 155);
                GravaMemDados(Arquivo, 15, 44);
            break;
            case 102:
               printf("Leitura da memória %i", LeMemDados(Arquivo,1));
               printf("Leitura da memória %i", LeMemDados(Arquivo,10));
               printf("Leitura da memória %i", LeMemDados(Arquivo,15));
            break;            
        }
    } 
    return 0;
}

void Ler_Arquivo(){
    // Pergunta ao usuário o arquivo a ser carregado
    char nome_arquivo[100];
    char line[Tam_Instrucao];
    instrucoes = NULL;
    instrucoes = alocaMemoria(Limite_de_Instrucoes);
    int opcode;
    printf("Digite o nome do arquivo a ser aberto\n");
    scanf("%99s", nome_arquivo);

    // Abre o arquivo
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Memoria Intrucoes[Limite_de_Instrucoes];
    char buffer[5];
    //O loop vai coletar as instruções em enquanto houver algo nas linhas 
    while (fgets(line, sizeof(line), file)) {
        // Remove caracteres extras como novas linhas e espaços
        line[strcspn(line, "\r\n")] = 0;
        
     
          
        strncpy(buffer, line, 4);
        buffer[4] = '\0';
        //printf("%s \n", buffer);
        opcode = converteOpCode(buffer);
        //printf("%d \n", opcode);
        if(opcode==0){
            *(instrucoes+cont) = recebeInstrucaoR(line);
        } else if (opcode==2){
            *(instrucoes+cont) = recebeInstrucaoJ(line);
        } else if (opcode == 4 || opcode == 8 || opcode == 11 || opcode == 15) {
            *(instrucoes+cont) = recebeInstrucaoI(line);
        }
        
       // Evita armazenar linhas vazias
        if (strlen(line) > 0) {
            cont++;
        }
        //Limita quantas instruções vão ser guardadas no programa
        if (cont >= Limite_de_Instrucoes) {
            break;
        }
        
        
        //printf("fim volta \n\n");
        //printf("contador: %d \n", cont);
    }
    printf("Arquivo carregado com sucesso...\n");
    fclose(file);
}

int converteOpCode(char *line) {
    int decimal = 0;
     for (int i = 0; i < 4; i++) {
        if (line[i] == '1') {
            decimal += pow(2, 4 - 1 - i);
            //printf("%d \n", decimal);
        } else if (line[i] != '0') {
             printf("Erro: Entrada inválida. Use apenas '0' e '1'.\n");
            return -1;
        }
    }
    return decimal;
}

int binParaDec(char *line) {   
    int decimal = 0;
    int tamanho = strlen(line);
     for (int i = 0; i < tamanho; i++) {
        if (line[i] == '1') {
            decimal += pow(2, tamanho - 1 - i);
        } else if (line[i] != '0') {
             printf("Erro: Entrada inválida. Use apenas '0' e '1'.\n");
            return -1;
        }
    }
    return decimal;
}

Memoria recebeInstrucaoR(char *line) {
    Memoria instrucao;
    
    instrucao.imm = -1;
    instrucao.end = -1;

    strcpy(instrucao.binario, line);
    
    strncpy(instrucao.sop, instrucao.binario, 4);
    instrucao.sop[4] = '\0';
    instrucao.op = binParaDec(instrucao.sop);

    strncpy(instrucao.srs, instrucao.binario + 4, 3);
    instrucao.srs[3] = '\0';
    instrucao.rs = binParaDec(instrucao.srs);

    strncpy(instrucao.srt, instrucao.binario + 7, 3);
    instrucao.srt[3] = '\0';
    instrucao.rt = binParaDec(instrucao.srt);

    strncpy(instrucao.srd, instrucao.binario + 10, 3);
    instrucao.srd[3] = '\0';
    instrucao.rd = binParaDec(instrucao.srd);
    
    strncpy(instrucao.sfunct, instrucao.binario + 13, 3);
    instrucao.sfunct[3] = '\0';
    instrucao.funct = binParaDec(instrucao.sfunct);

    instrucao.tipo = TipoR;
    return instrucao;
}

Memoria recebeInstrucaoI(char *line) {
    Memoria instrucao;
    
    instrucao.rd=-1;
    instrucao.funct=-1;
    instrucao.end=-1;

    strcpy(instrucao.binario, line);
    
    strncpy(instrucao.sop, instrucao.binario, 4);
    instrucao.sop[4] = '\0';
    instrucao.op = binParaDec(instrucao.sop);

    strncpy(instrucao.srs, instrucao.binario + 4, 3);
    instrucao.srs[3] = '\0';
    instrucao.rs = binParaDec(instrucao.srs);

    strncpy(instrucao.srt, instrucao.binario + 7, 3);
    instrucao.srt[3] = '\0';
    instrucao.rt = binParaDec(instrucao.srt);

    strncpy(instrucao.simm, instrucao.binario + 10, 6);
    instrucao.simm[6] = '\0';
    instrucao.imm = binParaDec(instrucao.simm);
    
    instrucao.tipo = TipoI;
    
     return instrucao;
}

Memoria recebeInstrucaoJ(char *line) {
    Memoria instrucao;
    
    instrucao.rt =-1;
    instrucao.rs =-1;
    instrucao.rd =-1;
    instrucao.funct = -1;
    instrucao.imm =-1;
    
    strcpy(instrucao.binario, line);
    
    strncpy(instrucao.sop, instrucao.binario, 4);
    instrucao.sop[4] = '\0';
    instrucao.op = binParaDec(instrucao.sop);
    
    strncpy(instrucao.send, instrucao.binario + 4, 12);
    instrucao.send[12] = '\0';
    instrucao.end = binParaDec(instrucao.send);
    
    instrucao.tipo = TipoJ;
     return instrucao;
}

void imprimeInstrucao(Memoria *instrucao){
    printf("\n \n  Binario da instrucao: %s \n \n", instrucao->binario);
     switch (instrucao->tipo) {
        case TipoR:
            printf("opcode: %d \n",  instrucao->op);
            printf("rs: %d \n",  instrucao->rs); 
            printf("rt: %d \n",  instrucao->rt);
            printf("rd: %d \n",  instrucao->rd);
        break;
        case TipoI:
            printf("opcode: %d \n",  instrucao->op);
            printf("rs: %d \n",  instrucao->rs); 
            printf("rt: %d \n",  instrucao->rt);
            printf("imediato : %d \n",  instrucao->imm);
        break;
        case TipoJ:
            printf("opcode: %d \n",  instrucao->op);
            printf("endereço: %d \n",  instrucao->end);
        break;
        default:
            printf("Outro\n");
        break;
    }
}

Memoria * alocaMemoria(int tam){
    Memoria* i;
    i = (Memoria*) malloc(tam*sizeof(Memoria));
    return i;
}

int ULA(int ulaop, int operando1, int operando2){
    switch (ulaop) {
        case 000: //ADD
            return operando1 + operando2;
        break;
        case 010: //SUB
            return operando1 - operando2;
        break;
        case 100: //AND
            return operando1 & operando2;
        break;
        case 101: //OR
            return operando1 | operando2;
        break;
    }
}

void escrever_registrador(int reg_destino, int valor) {
    if (reg_destino != 0) { //Nosso $0 irá SEMPRE ser 0
        registradores[reg_destino] = valor;
    }
}

void imprimir_registradores() {
    printf("=====Banco de Registradores=====\n");
    for (int i = 0; i < NUM_REGISTRADORES; i++) {
        printf("R%d: %d\n", i, registradores[i]);
    }
    printf("================================\n");
}

void incrementarpc(){
    pc++;
}

void modpc(int endereco){
    pc = endereco;
}

//Memória de Dados
void Verifica_Cria_MemDados(const char *arquivo) {
    FILE *MemoriaDados = fopen(arquivo, "rb"); // Apenas leitura para verificar existência

    // Verifica se o arquivo existe
    if (MemoriaDados) {
        printf("\nMemória de dados já existente.\n");
        fclose(MemoriaDados);
    } else {
        // Se não existe, cria
        MemoriaDados = fopen(arquivo, "wb+");
        if (MemoriaDados == NULL) {
            perror("Erro ao criar o arquivo");
            return;
        }
        printf("\nMemória de dados criada.\n");
        fclose(MemoriaDados);
    }
}

void GravaMemDados(const char *arquivo, int endereco, int dado) {
    FILE *MemoriaDados = fopen(arquivo, "r+b");
    if (!MemoriaDados) {
        perror("Erro ao abrir arquivo");
        return;
    }
    
    // Calcula a posição correta baseada no tamanho de um int
    long posicao = endereco * sizeof(int);

    fseek(MemoriaDados, posicao, SEEK_SET);
    fwrite(&dado, sizeof(int), 1, MemoriaDados);

    fclose(MemoriaDados);
}

int LeMemDados(const char *arquivo, int endereco) {
    FILE *MemoriaDados = fopen(arquivo, "rb");
    if (!MemoriaDados) {
        perror("Erro ao abrir arquivo");
        return -1; // Retorna um valor inválido para indicar erro
    }

    int auxiliar = 0;
    long posicao = endereco * sizeof(int);

    fseek(MemoriaDados, posicao, SEEK_SET);
    fread(&auxiliar, sizeof(int), 1, MemoriaDados);

    fclose(MemoriaDados);
    return auxiliar;
}
