#ifndef SCOREBOARDING_H
#define SCOREBOARDING_H

#include "../registers.h"
#include "../instructions.h"
#include "../clock.h"
#include "ula.h"
#include "../instructions_queue.h"
#include "scoreboarding_list.h"
#include <stdbool.h>

/*Estágios do scoreboarding*/
#define ISSUE 1
#define READ_OPERANDS 2
#define EXECUTION 3
#define WRITE_BACK 4

/*Se a unidade funcional está disponível ou não*/
#define UNAVAILABLE -1

/*Saltos*/
#define WRONG false
#define RIGHT true
int branch;

#define REGISTER_PC 34 //buffer para PC

Register registers_buffer[34]; //o buffer de PC está incluído aqui

void scoreboarding();
void issue(Instruction* instruction);
void read_operands(Instruction* instruction);
void execute(Instruction* instruction);
void write_back(Instruction* instruction);
int get_functional_unit(int opcode, int operation);
void clear_list();

#endif