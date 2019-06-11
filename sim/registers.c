#include "registers.h"
#include "clock.h"

/*Inicializa registradores*/
void initialize_registers(){
    for(int i = 0; i < 32; ++i){
        registers[i].data = 0;
        registers[i].busy = false;
    }
    write_register(clock, CLOCK_COUNT);/*Escreve o valor de clock no regitrador 9*/
}

/*Lê a informação contida em um registrador*/
int read_register(int index){
    return registers[index].data;
}

/*Escreve um dado, instrução,... em um registrador*/
bool write_register(int data, int index){
    if(!get_status(index)){
        registers[index].data = data;
        set_status(index);
        return true;
    }
    return false;
}

/*Obtém o status de um registrador*/
bool get_status(int index){
    return registers[index].busy;
}

/*muda o status de um registrador*/
int set_status(int index){
    registers[index].busy = (registers[index].busy)? false : true;
}