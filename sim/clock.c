#include "clock.h"

int clock;
int finish_clock = 0;

void init_clock(){
    clock = 0;
}

void increase_clock(){
    clock++;
}

void end_clock(){
    finish_clock = 1;
}