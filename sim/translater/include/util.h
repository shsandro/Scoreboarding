#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include "../translater.tab.h"
#include "../integration.h"

void translater(char* assembly_file);
int yyerror(char *s);
int yylex();

int get_register(char c, char i);
void write_r_instruction(int opcode, int rd, int rs, int rt, int funct);
void write_i_instruction(int opcode, int rs, int rt, int immediate);
void write_regimm_instruction(int opcode, int rs, int funct, int offset);
void write_j_instruction(int opcode, int target);


#endif