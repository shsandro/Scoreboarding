%{
#include "util.h"

#define SPECIAL_OPCODE 0b000000
#define SPECIAL2_OPCODE 0b011100
#define REGIMM_OPCODE 0b000001
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
%token COMMA EOL REGISTER IMMEDIATE LABEL 

/*Declaração dos tipos*/
%type <R_Instruction> r_instruction
%type <I_instrucition> i_instruction
%type <REGIMM_Instruction> regimm_instruction
%type <J_Instruction> j_instruction

%%

comma:
     | COMMA

eol:
  | eol EOL

instruction: r_instruction
           | i_instruction
           | regimm_instruction
           | j_instruction

r_instruction: ADD REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | AND REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | DIV REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.sham = 0; $$.funct = $1;}
            | JR REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | MFHI REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | MFLO REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | MOVN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | MOVZ REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | MTHI REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | MTLO REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | MULT REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.sham = 0; $$.funct = $1;}
            | NOP {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = 0; $$.rt = 0; $$.sham = 0; $$.funct = $1;}
            | NOR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | OR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | SUB REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | XOR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}
            | MADD REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.sham = 0; $$.funct = $1;}
            | MSUB REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.sham = 0; $$.funct = $1;}
            | MUL REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.sham = 0; $$.funct = $1;}

i_instruction: ADDI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | ANDI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | B IMMEDIATE {$$.opcode = $1; $$.rs = 0; $$.rt = 0; $$.immediate = $2;}
            | BEQ REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BEQL REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BGTZ REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BLEZ REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BNE REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | LUI REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = 0; $$.rt = $2; $$.immediate = $4;}
            | ORI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | XORI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}

regimm_instruction: BGEZ REGISTER comma IMMEDIATE {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}
            | BLTZ REGISTER comma IMMEDIATE {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}

j_instruction: J IMMEDIATE {$$.opcode = $1; $$.target = $2;}