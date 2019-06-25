#ifndef ULA_H
#define ULA_H

#include <stdbool.h>
#include "../instructions.h"

#define BUSY true
#define FREE false

/*Estágios do scoreboarding*/
#define ISSUE 1
#define READ_OPERANDS 2
#define EXECUTION 3
#define WRITE_RESULT 4

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

typedef struct{
    bool busy;
    int operation;
    int Fi; //registrador destino
    int Fj, Fk; //registradores origem
    int Qj, Qk; //UF dos regstradores que serão lidos
    int Rj, Rk; //indica se os operandos estão prontos
}Functional_Unit;

Functional_Unit functional_units[6];

int add_unit(int operation, int operand_1, int operand_2);
int sub_unit(int operand_1, int operand_2);
int mul_unit(int operand_1, int operand_2);
int div_unit(int operation, int operand_1, int operand_2);

#endif