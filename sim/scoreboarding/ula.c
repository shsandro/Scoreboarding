#include "ula.h"

/*realiza as operações da unidade funcional add*/
int add_unit(int operation, int operand_1, int operand_2){
    switch (operation) {
        case OP_ADD:
            return operand_1 + operand_2;
        case OP_AND:
            return operand_1 & operand_2;
        case OP_OR:
            return operand_1 | operand_2;
        case OP_NOR:
            return ~(operand_1 | operand_2);
        case OP_XOR:
            return operand_1 ^ operand_2;
        case OP_SL:
            return operand_1 << operand_2;
    }
}

/*realiza as operações da unidade funcional sub*/
int sub_unit(int operand_1, int operand_2){
    return operand_1 - operand_2;
}

/*realiza as operações das unidades funcionais de mul*/
int mul_unit(int operand_1, int operand_2){
    return operand_1 * operand_2;
}

/*realiza as operações das unidades funcionais de div*/
int div_unit(int operation, int operand_1, int operand_2){
    return (operation == OP_DIV)? operand_1 / operand_2 : operand_1 % operand_2;
}