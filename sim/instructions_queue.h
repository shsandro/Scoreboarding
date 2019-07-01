#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "instructions.h"
#include <stdlib.h>

#define EMPTY true
#define NOT_EMPTY false

typedef struct{
    int* instructions_queue;
    int num_instructions, begin, end;
    int max_instructions; //número máximo de instruções a serem buscadas é 4
}Queue;

Queue* Instructions_Queue;

void initialize_queue();
bool insert_instruction(int instruction);
bool remove_instruction();
bool get_status_queue();
int get_instruction();

#endif