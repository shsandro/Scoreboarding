#include "instructions_queue.h"

void initialize_queue(){
    Instructions_Queue = (Queue*)malloc(sizeof(Instructions_Queue));
    Instructions_Queue->instructions_queue = (int*)malloc(sizeof(int)*4);
    Instructions_Queue->num_instructions = 0;
    Instructions_Queue->max_instructions = 4;
}

bool insert_instruction(int instruction){
    if (Instructions_Queue->num_instructions == Instructions_Queue->max_instructions) return false;
    Instructions_Queue->instructions_queue[Instructions_Queue->num_instructions] = instruction;
    Instructions_Queue->num_instructions++;
    return true;
}

bool remove_instruction(){
    if (Instructions_Queue->num_instructions == 0) return false;
    Instructions_Queue->num_instructions--;
    return true;
}

int get_instruction(){
    return (Instructions_Queue->num_instructions == 0)? false : Instructions_Queue->instructions_queue[Instructions_Queue->num_instructions];
}

bool get_status_queue(){
    return (Instructions_Queue->num_instructions == 0)? EMPTY : NOT_EMPTY;
}