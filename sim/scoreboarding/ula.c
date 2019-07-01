#include "ula.h"

void initialize_functional_units(){
    for (int i = 0; i < 6; ++i){
        functional_units[i].busy = false;
        functional_units[i].operation = NONE;
        functional_units[i].Fi = NONE;
        functional_units[i].Fj = NONE;
        functional_units[i].Fk = NONE;
        functional_units[i].Qj = NONE;
        functional_units[i].Qk = NONE;
        functional_units[i].Rj = true;
        functional_units[i].Rk = true;
        functional_units[i].result[0] = 0;
        functional_units[i].result[1] = 0;
    }
}

void reinitialize_unit(int functional_unit){
    functional_units[functional_unit].busy = false;
    functional_units[functional_unit].operation = NONE;
    functional_units[functional_unit].Fi = NONE;
    functional_units[functional_unit].Fj = NONE;
    functional_units[functional_unit].Fk = NONE;
    functional_units[functional_unit].Qj = NONE;
    functional_units[functional_unit].Qk = NONE;
    functional_units[functional_unit].Rj = true;
    functional_units[functional_unit].Rk = true;
    functional_units[functional_unit].result[0] = 0;
    functional_units[functional_unit].result[1] = 0;
}

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
    if (operand_2 == 0) return 0;
    return (operation == OP_DIV)? (int)(operand_1 / operand_2): operand_1 % operand_2;
}