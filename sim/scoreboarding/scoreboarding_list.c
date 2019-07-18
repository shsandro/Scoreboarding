#include "scoreboarding_list.h"

void initialize_list(){
    scoreboarding_list.list  = (Instruction**)malloc(sizeof(Instruction*)*7);
    for (int i = 0; i < 7; ++i){
        scoreboarding_list.list[i] = (Instruction*)malloc(sizeof(Instruction*));
        scoreboarding_list.list[i] = NULL;
    }
    scoreboarding_list.num_instructions = 0;
    scoreboarding_list.max_instructions = 7;
}

bool insert_scoreboardig(Instruction* instruction){
    if (scoreboarding_list.num_instructions == scoreboarding_list.max_instructions) return false;
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL){
            scoreboarding_list.list[i] = instruction;
            scoreboarding_list.num_instructions++;
            return true;
        }
    } 
}

bool remove_scoreboarding(Instruction* instruction){
    if (scoreboarding_list.num_instructions == 0) return false;
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == instruction){
            scoreboarding_list.list[i] = NULL;
            scoreboarding_list.num_instructions--;
            return true;
        }
    } 
}

bool get_status_list(){
    printf("num ins list %d\n", scoreboarding_list.num_instructions);
    return (scoreboarding_list.num_instructions == 0)? EMPTY : NOT_EMPTY;
}

