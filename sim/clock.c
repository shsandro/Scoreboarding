#include "./include/clock.h"

/*Inicializa o clock*/
void initialize_clock(){
    clock = 0;
    finish_clock = 0;
}

/*Incrementa o clock*/
void increase_clock(){
    ++clock;
}

/*Retorna o valor atual do clock*/
int get_clock(){
    return clock;
}