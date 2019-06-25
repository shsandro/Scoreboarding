#include"cpu.h"
#include "registers.h"
#include "memory.h"
#include "./scoreboarding/scoreboarding.h"

typedef struct{
    unsigned x: 6; // 10 bits
}Opcode;

/*Busca próxima instrução e marmazena em IR*/
void fetch_stage(){
    IR.data = read_mem(PC.data);
    PC.data += 4;
}

void execution_stage(){
    Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
    scoreboarding(instruction, IR.data);
}