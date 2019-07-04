#ifndef SCOREBOARDING_LIST_H
#define SCOREBOARDING_LIST_H

#include "../instructions.h"
#include <stdlib.h>
#include <stdbool.h>

#define EMPTY true
#define NOT_EMPTY false

typedef struct{
    Instruction**  list;
    int num_instructions, max_instructions;
}Scoreboarding_list;

Scoreboarding_list scoreboarding_list;


void initialize_list();
bool insert_scoreboardig(Instruction* instruction);
bool remove_scoreboarding(Instruction* instruction);
bool get_status_list();

#endif