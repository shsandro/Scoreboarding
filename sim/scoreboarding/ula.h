#ifndef ULA_H
#define ULA_H

#include <stdbool.h>
#include "../instructions.h"

#define BUSY true
#define FREE false
#define ISSUE 1
#define READ_OPERANDS 2
#define EXECUTION 3
#define WRITE_RESULT 4

typedef struct{
    bool mul[2];
    bool div[2];
    bool sub;
    bool add;
}Ula;

Ula functional_units;

int logical_operations(int operation, int operand_1, int operand_2);
int arithmetic_operations(int operation, int operand_1, int operand_2);

#endif