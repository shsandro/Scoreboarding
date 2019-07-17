%{
#include "util.h"
#include "hash.h"

#define SPECIAL_OPCODE 0b000000
#define SPECIAL2_OPCODE 0b011100
#define REGIMM_OPCODE 0b000001

extern FILE *yyin;
int PC = 0;
int num_instructions = 0;
int second_pass = 0;
FILE* output;
%}

%union{
  struct R{
    int opcode, rd, rs, rt, shamt, funct;
  }R_Instruction;
  struct I{
    int opcode, rs, rt, immediate;
  }I_instrucition;
  struct REGIMM{
    int opcode, rs, funct, offset;
  }REGIMM_Instruction;
  struct J{
    int opcode, target;
  }J_Instruction;
  int value;
  char *str;
}

/*Declaração dos tokens*/
//Instruções imediatas
%token <value> ADDI ANDI B BEQ BEQL BGTZ BLEZ BNE LUI ORI XORI
//Jump
%token <value> J
//Instruções SPECIAL E SPECIAL2
%token <value> ADD AND DIV JR MFHI MFLO MOVN MOVZ MTHI MTLO MULT NOP NOR OR SUB XOR MADD MSUB MUL
//Instruções REGIMM
%token <value> BGEZ BLTZ
//Outros
%token <value> REGISTER IMMEDIATE
%token <value> LABEL
%token COMMA EOL

/*Declaração dos tipos*/
%type <R_Instruction> r_instruction
%type <I_instrucition> i_instruction
%type <REGIMM_Instruction> regimm_instruction
%type <J_Instruction> j_instruction

%%

all: 
    | all instruction eol
    | all LABEL eol

comma:
     | COMMA

eol:
  | eol EOL

instruction: r_instruction {if(second_pass){num_instructions++; write_r_instruction($1.opcode, $1.rd, $1.rs, $1.rt, $1.funct);}else{num_instructions++;}}
           | i_instruction {if(second_pass){num_instructions++; write_i_instruction($1.opcode, $1.rs, $1.rt, $1.immediate);}else{num_instructions++;}}
           | regimm_instruction {if(second_pass){num_instructions++; write_regimm_instruction($1.opcode, $1.rs, $1.funct, $1.offset);}else{num_instructions++;}}
           | j_instruction {if(second_pass){num_instructions++; write_j_instruction($1.opcode, $1.target);}else{num_instructions++;}}

r_instruction: ADD REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | AND REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | DIV REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | JR REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MFHI REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MFLO REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MOVN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MOVZ REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MTHI REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MTLO REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MULT REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | NOP {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | NOR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | OR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | SUB REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | XOR REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MADD REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | MSUB REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | MUL REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}

i_instruction: ADDI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | ANDI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | B LABEL {$$.opcode = $1; $$.rs = 0; $$.rt = 0; $$.immediate = $2;}
            | BEQ REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BEQL REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BGTZ REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BLEZ REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BNE REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | LUI REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = 0; $$.rt = $2; $$.immediate = $4;}
            | ORI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | XORI REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}

regimm_instruction: BGEZ REGISTER comma LABEL {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}
            | BLTZ REGISTER comma LABEL {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}

j_instruction: J LABEL {$$.opcode = $1; $$.target = $2;}

%%

void main()
{
    FILE* input;
    char file[100];
    printf("Digite o nome do arquivo de assembly: ");
	  fgets(file, 99, stdin);
	  file[strlen(file)-1] = '\0';
    input = fopen(file, "r");

    instructions = fopen("instructions.txt", "wr+");

    init_hash();
    yyin = input;
    yyparse();
    rewind(input);
    second_pass = 1;
    num_instructions = 0;
    printf("\nComeçando segunda passada\n\n");
    yyparse();
    fclose(input);
    fclose(instructions);
}

void yyerror(const char *s) {
  printf("EEK, parse error! Message: %s\n", s);
  // might as well halt now:
  exit(-1);
}