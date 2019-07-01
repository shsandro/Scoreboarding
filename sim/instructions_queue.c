#include "instructions_queue.h"
#include <stdio.h>
void initialize_queue(){
    Instructions_Queue = (Queue*)malloc(sizeof(Instructions_Queue));
    Instructions_Queue->instructions_queue = (int*)malloc(sizeof(int)*4);
    Instructions_Queue->num_instructions = 0;
    Instructions_Queue->begin = 0;
    Instructions_Queue->end = 0;
    Instructions_Queue->max_instructions = 4;
}

bool insert_instruction(int instruction){
    if (Instructions_Queue->end == Instructions_Queue->max_instructions) return false;
    printf("Inseri na posição: %d\n", Instructions_Queue->end);
    Instructions_Queue->instructions_queue[Instructions_Queue->end] = instruction;
    Instructions_Queue->end++;
    Instructions_Queue->num_instructions++;
    return true;
}

bool remove_instruction(){
    if (Instructions_Queue->num_instructions == 0) return false;
    Instructions_Queue->num_instructions--;
    return true;
}

int get_instruction(){
    if (Instructions_Queue->num_instructions <= 0){
        return false;
    } else if (Instructions_Queue->num_instructions == 1){
        int instruction = Instructions_Queue->instructions_queue[Instructions_Queue->begin];
        Instructions_Queue->end = 0;
        Instructions_Queue->begin = 0;
        Instructions_Queue->num_instructions--;
        return instruction;
    } else {
        int instruction = Instructions_Queue->instructions_queue[Instructions_Queue->begin];
        Instructions_Queue->begin++;
        Instructions_Queue->num_instructions--;
        return instruction;
    }
}

bool get_status_queue(){
    return (Instructions_Queue->num_instructions == 0)? EMPTY : NOT_EMPTY;
}