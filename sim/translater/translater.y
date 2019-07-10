%{
#include "util.h"
%}

%union{
  typedef struct{
    int opcode, rd, rs, rt, shamt, funct;
  }R_Instruction;

  typedef struct{
    int opcode, rs, rt, immediate;
  }I_instrucition;

  typedef struct{
    int opcode, rs, funct, offset;
  }REGIMM_Instruction;

  typedef struct{
    int opcode, target;
  }J_Instruction;

  int value;
  char *str;
}

/*Declaração dos tokens*/
//Instruções imediatas
%token ADDI ANDI B BEQ BEQL BGTZ BLEZ BNE LUI ORI XORI
//Jump
%token JUMP
//Instruções SPECIAL E SPECIAL2
%token ADD AND DIV JR MFHI MFLO MOVN MOVZ MTHI MTLO MULT NOP NOR OR SUB XOR MADD MSUB MUL
//Instruções REGIMM
%token BGEZ BLTZ
//Outros
%token COMMA EOL REGISTER LABEL 

/*Declaração dos tipos*/
%type <R_Instruction> r_instruction
%type <I_instrucition> i_instruction
%type <REGIMM_Instruction> regimm_instruction
%type <J_Instruction> j_instruction

%%

instruction: r_instruction
           | i_instruction
           | regimm_instruction
           | j_instruction