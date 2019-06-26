#include"cpu.h"

typedef struct{
    unsigned x: 6; // 10 bits
}Opcode;

/*Busca próxima instrução, marmazena em IR e coloca na fila de instruções*/
void fetch_stage(){
    if (get_status_queue() == EMPTY){
        int instructions_fetched = 0;
        while(instructions_fetched < 4 && PC.data < finish_PC) {
            IR.data = read_mem(PC.data);
            insert_instruction(IR.data);
            PC.data += 4;
            ++instructions_fetched;
        }
    }
}

void execution_stage(){
    if (get_status_queue() == NOT_EMPTY){
        //Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
        scoreboarding();
    }
}