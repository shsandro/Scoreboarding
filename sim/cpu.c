#include<stdio.h>
#include "registers.h"

int fetch_stage(){
    IR.data = mem_read(PC.data);
    PC.data += 4; //Desloca uma posição de PC  
}

void decode_stage(){
    int instruction =  IR.data;
    int opcode = instruction 
}