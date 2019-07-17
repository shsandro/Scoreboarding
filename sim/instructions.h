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
#define I_ADDI 8
#define I_ANDI 12
#define I_BEQ 4
#define I_BEQL 20
#define I_BGTZ 7
#define I_BLEZ 6
#define I_BNE 5
#define I_LUI 15
#define I_ORI 13
#define I_XORI 14

/*Definições dos opcodes das instruções REGIMM*/
#define I_BGEZ 1 
#define I_BLTZ 0

/*Definições dos opcodes das instruções SPECIAL*/
#define I_ADD 32
#define I_AND 36
#define I_DIV 26
#define I_JR 9
#define I_MFHI 16
#define I_MFLO 18
#define I_MOVN 11
#define I_MOVZ 10
#define I_MTHI 17
#define I_MTLO 19
#define I_MULT 24
#define I_NOP 0
#define I_NOR 39
#define I_OR 37
#define I_SUB 34
#define I_XOR 38

/*Definições dos opcodes das instruções SPECIAL2*/
#define I_MADD 0
#define I_MSUB 4
#define I_MUL 2

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
    int functional_unit;
    int execution_begin;
    bool comes_from_branch;
    R_Instruction r_instruction;
    I_Instruction i_instruction;
    REGIMM_Instruction regimm_instruction;
    J_Instruction j_instruction;
}Instruction;

#endif