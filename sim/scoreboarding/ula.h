#ifndef ULA_H
#define ULA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../instructions.h"

#define BUSY true
#define FREE false
#define NONE -1

/*tipos de unidades funcionais*/
#define MUL 0
#define DIV 1
#define ADD 2
#define SUB 3

/*id/posição das unidades funcionais*/
#define UF_MUL1 0
#define UF_MUL2 1
#define UF_DIV1 2
#define UF_DIV2 3
#define UF_SUB 4
#define UF_ADD 5 //também realiza as operações lógicas

/*operações das unidades funcionais*/
#define OP_ADD 0
#define OP_AND 1
#define OP_OR 2
#define OP_NOR 3
#define OP_XOR 4 
#define OP_SL 5
#define OP_SUB 6
#define OP_MUL 7
#define OP_DIV 8
#define OP_MOD 9

/*Qauntidade de ciclos para cada operação*/
#define BRANCH_CYCLES 1
#define ADD_CYCLES 10
#define SUB_CYCLES 10
#define AND_CYCLES 2
#define OR_CYCLES 2
#define NOR_CYCLES 2
#define XOR_CYCLES 2
#define SL_CYCLES 1
#define MUL_CYCLES 40
#define DIV_CYCLES 40
#define MOD_CYCLES 40

typedef struct{
    bool busy;
    int operation;
    int Fi; //registrador destino
    int Fj, Fk; //registradores origem
    int Qj, Qk; //UF dos regstradores que serão lidos
    bool Rj, Rk; //indica se os operandos estão prontos
    int result[2]; //resultado da operação, pode guardar HIGH e LOW dependendo da operação
}Functional_Unit;

Functional_Unit functional_units[6];

void initialize_functional_units();
void reinitialize_unit(int functional_unit);
int add_unit(int operation, int operand_1, int operand_2);
int sub_unit(int operand_1, int operand_2);
int mul_unit(int operand_1, int operand_2);
int div_unit(int operation, int operand_1, int operand_2);

#endif