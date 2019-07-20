#include "scoreboarding_list.h"

/*Incicializa a fila/barramento de instruções do scoreboarding*/
void initialize_list(){
    scoreboarding_list.list  = (Instruction**)malloc(sizeof(Instruction*)*7);
    for (int i = 0; i < 7; ++i){
        scoreboarding_list.list[i] = (Instruction*)malloc(sizeof(Instruction*));
        scoreboarding_list.list[i] = NULL;
    }
    scoreboarding_list.num_instructions = 0;
    scoreboarding_list.max_instructions = 7;
}

/*Insere uma instrução no scoreboarding*/
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

/*Remove uma instrução do scoreboarding. É chamada no estágio de write_back*/
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

/*Retorna se o barramento do scoreboarding está vazio ou não*/
bool get_status_list(){
    return (scoreboarding_list.num_instructions == 0)? EMPTY : NOT_EMPTY;
}

