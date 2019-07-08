#include "instructions_queue.h"
#include <stdio.h>

void initialize_queue(){
    Instructions_Queue = (Queue*)malloc(sizeof(Instructions_Queue));
    Instructions_Queue->instructions_queue  = (Instruction**)malloc(sizeof(Instruction*)*4);
    for (int i = 0; i < 4; ++i){
        Instructions_Queue->instructions_queue[i] = (Instruction*)malloc(sizeof(Instruction*));
        Instructions_Queue->instructions_queue[i] = NULL;
    }
    Instructions_Queue->num_instructions = 0;
    Instructions_Queue->begin = 0;
    Instructions_Queue->end = 0;
    Instructions_Queue->max_instructions = 4;
}

bool insert_instruction(Instruction* instruction){
    if (Instructions_Queue->end == Instructions_Queue->max_instructions) return false;
    Instructions_Queue->instructions_queue[Instructions_Queue->end] = instruction;
    Instructions_Queue->end++;
    Instructions_Queue->num_instructions++;
    return true;
}

void clear_queue(){
    for (int i = 0; i < 4; ++i){
        free(Instructions_Queue->instructions_queue[i]);
        Instructions_Queue->instructions_queue[i] = NULL;
    }
    Instructions_Queue->num_instructions = 0;
    Instructions_Queue->begin = 0;
    Instructions_Queue->end = 0;
}

Instruction* get_instruction(){
    if (Instructions_Queue->num_instructions <= 0){
        return false;
    } else if (Instructions_Queue->num_instructions == 1){
        Instruction* instruction = Instructions_Queue->instructions_queue[Instructions_Queue->begin];
        Instructions_Queue->instructions_queue[Instructions_Queue->begin] = NULL;
        printf("retornando um %d na pos %d\n", instruction->opcode, Instructions_Queue->begin);
        Instructions_Queue->end = 0;
        Instructions_Queue->begin = 0;
        Instructions_Queue->num_instructions--;
        return instruction;
    } else {
        Instruction* instruction = Instructions_Queue->instructions_queue[Instructions_Queue->begin];
        Instructions_Queue->instructions_queue[Instructions_Queue->begin] = NULL;
        Instructions_Queue->begin++;
        Instructions_Queue->num_instructions--;
        printf("retornando um %d na pos %d\n", instruction->opcode, Instructions_Queue->begin-1);
        return instruction;
    }
}

bool get_status_queue(){
    return (Instructions_Queue->num_instructions == 0)? EMPTY : NOT_EMPTY;
}