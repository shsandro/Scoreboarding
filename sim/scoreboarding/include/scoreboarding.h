#ifndef SCOREBOARDING_H
#define SCOREBOARDING_H

#include "scoreboarding_list.h"
#include "ula.h"
#include "../../include/registers.h"
#include "../../include/instructions.h"
#include "../../include/clock.h"
#include "../../include/instructions_queue.h"
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

int instructions_written;
int branches_taken;
int branches_hit;
int branches_miss;
void scoreboarding();
void run_scoreboarding();
void issue(Instruction* instruction);
void read_operands(Instruction* instruction);
void execute(Instruction* instruction);
void write_back(Instruction* instruction);
int get_functional_unit(int opcode, int operation);
void bypass(int result_register, int data);
void clear_list(Instruction* instruction);

#endif