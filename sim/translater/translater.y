%{
#include "util.h"
#include "hash.h"
#include "integration.h"

#define SPECIAL_OPCODE 0b000000
#define SPECIAL2_OPCODE 0b011100
#define REGIMM_OPCODE 0b000001

extern FILE *yyin;
extern FILE *input;
extern FILE *output;
int instructions_count = 0;
int second_pass = 0;
%}

%union{
  struct R{
    int opcode, rd, rs, rt, shamt, funct;
  }R_Instruction;
  struct I{
    int opcode, rs, rt, immediate;
  }I_instrucition;
  struct REGIMM_R{
    int opcode, rs, funct, offset;
  }REGIMM_Instruction;
  struct JUMP{
    int opcode, target;
  }J_Instruction;
  int value;
  char *str;
}

/*Declaração dos tokens*/
//Instruções imediatas
%token <value> ADDI_TOKEN ANDI_TOKEN B_TOKEN BEQ_TOKEN BEQL_TOKEN BGTZ_TOKEN BLEZ_TOKEN BNE_TOKEN LUI_TOKEN ORI_TOKEN XORI_TOKEN
//Jump
%token <value> JUMP_TOKEN
//Instruções SPECIAL E SPECIAL2
%token <value> ADD_TOKEN AND_TOKEN DIV_TOKEN JR_TOKEN MFHI_TOKEN MFLO_TOKEN MOVN_TOKEN MOVZ_TOKEN MTHI_TOKEN MTLO_TOKEN MULT_TOKEN NOP_TOKEN NOR_TOKEN OR_TOKEN SUB_TOKEN XOR_TOKEN MADD_TOKEN MSUB_TOKEN MUL_TOKEN
//Instruções REGIMM
%token <value> BGEZ_TOKEN BLTZ_TOKEN
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
    | all eol

comma:
     | COMMA

eol:
  | eol EOL

instruction: r_instruction {if(second_pass){instructions_count++; write_r_instruction($1.opcode, $1.rd, $1.rs, $1.rt, $1.funct);}else{instructions_count++;}}
           | i_instruction {if(second_pass){instructions_count++; write_i_instruction($1.opcode, $1.rs, $1.rt, $1.immediate);}else{instructions_count++;}}
           | regimm_instruction {if(second_pass){instructions_count++; write_regimm_instruction($1.opcode, $1.rs, $1.funct, $1.offset);}else{instructions_count++;}}
           | j_instruction {if(second_pass){instructions_count++; write_j_instruction($1.opcode, $1.target);}else{instructions_count++;}}

r_instruction: ADD_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | AND_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | DIV_TOKEN REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | JR_TOKEN REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MFHI_TOKEN REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MFLO_TOKEN REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MOVN_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MOVZ_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MTHI_TOKEN REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MTLO_TOKEN REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | MULT_TOKEN REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | NOP_TOKEN {$$.opcode = SPECIAL_OPCODE; $$.rd = 0; $$.rs = 0; $$.rt = 0; $$.shamt = 0; $$.funct = $1;}
            | NOR_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | OR_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | SUB_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | XOR_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}
            | MADD_TOKEN REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | MSUB_TOKEN REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = 0; $$.rs = $2; $$.rt = $4; $$.shamt = 0; $$.funct = $1;}
            | MUL_TOKEN REGISTER comma REGISTER comma REGISTER {$$.opcode = SPECIAL2_OPCODE; $$.rd = $2; $$.rs = $4; $$.rt = $6; $$.shamt = 0; $$.funct = $1;}

i_instruction: ADDI_TOKEN REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rt = $2; $$.rs = $4; $$.immediate = $6;}
            | ANDI_TOKEN REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rt = $2; $$.rs = $4; $$.immediate = $6;}
            | B_TOKEN LABEL {$$.opcode = $1; $$.rs = 0; $$.rt = 0; $$.immediate = $2;}
            | BEQ_TOKEN REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BEQL_TOKEN REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | BGTZ_TOKEN REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BLEZ_TOKEN REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = 0; $$.immediate = $4;}
            | BNE_TOKEN REGISTER comma REGISTER comma LABEL {$$.opcode = $1; $$.rs = $2; $$.rt = $4; $$.immediate = $6;}
            | LUI_TOKEN REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rs = 0; $$.rt = $2; $$.immediate = $4;}
            | ORI_TOKEN REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rt = $2; $$.rs = $4; $$.immediate = $6;}
            | XORI_TOKEN REGISTER comma REGISTER comma IMMEDIATE {$$.opcode = $1; $$.rt = $2; $$.rs = $4; $$.immediate = $6;}

regimm_instruction: BGEZ_TOKEN REGISTER comma LABEL {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}
            | BLTZ_TOKEN REGISTER comma LABEL {$$.opcode = REGIMM_OPCODE; $$.rs = $2; $$.funct = $1; $$.offset = $4;}

j_instruction: JUMP_TOKEN LABEL {$$.opcode = $1; $$.target = $2;}

%%

/*Realiza a tradução do arquivo assembly. Gera como saída o arquivo binario output.bin. Foi escrito com Flex & Bison*/
void translater(char* assembly_file)
{
    input_assembly = fopen(assembly_file, "r");
    
    if(!input_assembly){
      printf("Arquivo inexistente.\n");
      exit(-1);
    }

    output = fopen("output.bin", "wr+");

    init_hash();
    yyin = input_assembly;
    printf("Identificando labels...\n\n");
    yyparse();
    rewind(input_assembly);
    second_pass = 1;
    instructions_count = 0;
    printf("Traduzindo...\n\n");
    yyparse();
    fclose(input_assembly);
    fclose(output);
    printf("Sucesso na tradução!\n\n");
}

void yyerror(const char *s) {
  printf("EEK, parse error! Message: %s\n", s);
  // might as well halt now:
  exit(-1);
}