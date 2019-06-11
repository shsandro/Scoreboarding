#include"cpu.h"
#include "registers.h"

/*Busca próxima instrução e marmazena em IR*/
void fetch_stage(){
    IR.data = mem_read(PC.data);
    PC.data += 4;  
}

void decode_stage(){
    int instruction =  IR.data;
    int opcode = instruction 
}