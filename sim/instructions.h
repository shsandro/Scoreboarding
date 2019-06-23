#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>

/*Máscaras*/
#define MASK_TYPE 63
#define MASK_REGISTER 31
#define MASK_OFFSET 65535
#define MASK_TARGET 67108863


/*Tipos de instruções*/
#define SPECIAL 0
#define SPECIAL2 28
#define REGIMM 1
#define J 2

/*Definições dos opcodes das instruções imediatas*/
#define ADDI 8
#define ANDI 12
#define BEQ 4
#define BEQL 20
#define BGTZ 7
#define BLEZ 6
#define BNE 5
#define LUI 15
#define ORI 13
#define XORI 14

/*Definições dos opcodes das instruções REGIMM*/
#define BGEZ 1 
#define BLTZ 0

/*Definições dos opcodes das instruções SPECIAL*/
#define ADD 32
#define AND 36
#define DIV 26
#define JR 9
#define MFHI 16
#define MFLO 18
#define MOVN 11
#define MOVZ 10
#define MTHI 17
#define MTLO 19
#define MULT 24
#define NOP 0
#define NOR 39
#define OR 37
#define SUB 34
#define XOR 38

/*Definições dos opcodes das instruções SPECIAL2*/
#define MADD 0
#define MSUB 4
#define MUL 2

/*Para instruções dos tipos SPECIAL e SPECIAL2*/
typedef struct{
    int rs, rt, rd;
    int shamt;
    int funct;
}R_Instruction;

/*Para instruções do tipo imediata*/
typedef struct{
    int rs, rt;
    int immediate;
}I_Instruction;

/*Para instruções do tipo REGIMM*/
typedef struct{
    int rs;
    int funct;
    int offset;
}REGIMM_Instruction;

/*Para instruções do tipo J*/
typedef struct{
    int target;
}J_Instruction;

/*Instrução geral para ser adicionada na fila*/
typedef struct{
    int opcode;
    int stage;
    R_Instruction r_instruction;
    I_Instruction i_instruction;
    REGIMM_Instruction regimm_instruction;
    J_Instruction j_instruction;
}Instruction;

#endif