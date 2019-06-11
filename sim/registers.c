#include "registers.h"

/*Lê a informação contida em um registrador*/
int read_register(int register){
    return registers[register].data;
}

/*Escreve um dado, instrução,... em um registrador*/
bool write_register(int data, int register){
    if(!get_status(register)){
        registers[register].data = data;
        set_status(register);
        return true;
    }
    return false;
}

/*Obtém o status de um registrador*/
bool get_status(int register){
    return registers[register].busy;
}

/*muda o status de um registrador*/
int set_status(int register){
    registers[register].busy = (registers[register].busy)? false : true;
}