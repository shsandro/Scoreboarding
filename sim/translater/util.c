#include "util.h"

/*Retorna o número de um registrador*/
int get_register(char c, char i){
    switch (c)
    {
    case 'v':
        switch (i)
        {
        case '0':
            return 2;
        case '1':
            return 3;
        }
    case 'a':
        switch (i)
        {
        case '0':
            return 4;
        case '1':
            return 5;
        case '2':
            return 6;
        case '3':
            return 7;
        }
    case 't':
        switch (i)
        {
        case '0':
            return 8;
        case '1':
            return 9;
        case '2':
            return 10;
        case '3':
            return 11;
        case '4':
            return 12;
        case '5':
            return 13;
        case '6':
            return 14;
        case '7':
            return 15;
        case '8':
            return 24;
        case '9':
            return 25;
        }
    case 's':
        switch (i)
        {
        case '0':
            return 16;
        case '1':
            return 17;
        case '2':
            return 18;
        case '3':
            return 19;
        case '4':
            return 20;
        case '5':
            return 21;
        case '6':
            return 22;
        case '7':
            return 23;
        }
    case 'k':
        switch (i)
        {
        case '0':
            return 26;
        case '1':
            return 27;
        }
    default:
        printf("Registrador desconhecido.");
        exit(EXIT_FAILURE);
    }
}

/*Escreve uma instrução dto tipo R no arquivo output.bin*/
void write_r_instruction(int opcode, int rd, int rs, int rt, int funct){
    printf("Escrevendo uma R\n");
    int instruction = opcode; instruction = instruction << 26;
    rs = rs << 21; instruction = instruction | rs;
    rt = rt << 16; instruction = instruction | rt;
    rd = rd << 11; instruction = instruction | rd;
    instruction = instruction | funct;
    printf("Instrução %d\n", instruction);
    fwrite(&instruction, 4, 1, output);
}

/*Escreve uma instrução dto tipo I no arquivo output.bin*/
void write_i_instruction(int opcode, int rs, int rt, int immediate){
    printf("Escrevendo uma I\n");
    int instruction = opcode; instruction = instruction << 26;
    rs = rs << 21; instruction = instruction | rs;
    rt = rt << 16; instruction = instruction | rt;
    instruction = instruction | immediate;
    printf("Instrução %d\n", instruction);
    fwrite(&instruction, 4, 1, output);
}

/*Escreve uma instrução dto tipo REGIMM no arquivo output.bin*/
void write_regimm_instruction(int opcode, int rs, int funct, int offset){
    printf("Escrevendo uma Regimm\n");
    int instruction = opcode; instruction = instruction << 26;
    rs = rs << 21; instruction = instruction | rs;
    funct = funct << 16; instruction = instruction | funct;
    instruction = instruction | offset;
    printf("Instrução %d\n", instruction);
    fwrite(&instruction, 4, 1, output);
}

/*Escreve uma instrução dto tipo J no arquivo output.bin*/
void write_j_instruction(int opcode, int target){
    printf("Escrevendo uma J\n");
    int instruction = opcode; instruction = instruction << 26;
    instruction = instruction | target;
    printf("Instrução %d\n", instruction);
    fwrite(&instruction, 4, 1, output);
}